#include <Arduino.h>
#include <SPI.h>
#include "stdint.h"
#include "invClockGen.h"
#include "invDMASPI.h"


extern "C" typedef struct                                                                // DMAC descriptor structure
{
  uint16_t btctrl;
  uint16_t btcnt;
  uint32_t srcaddr;
  uint32_t dstaddr;
  uint32_t descaddr;
} dmacdescriptor ;

volatile dmacdescriptor wrb[12] __attribute__ ((aligned (16)));               // Write-back DMAC descriptors
dmacdescriptor descriptor_section[12] __attribute__ ((aligned (16)));         // DMAC channel descriptors
dmacdescriptor descriptor __attribute__ ((aligned (16)));                     // Place holder descriptor

extern "C" volatile uint8_t spi_buffer_tx[2] = {0, 0};
extern "C" volatile uint8_t spi_buffer_rx[2] = {0, 0};


extern "C" void invDMASPI_init(void) { //SPI init

  if (invClockGen_status() == 0)    //if generic clocks are not running; start them
  {
    invClockGen_init();         
  }

 // 1. Clock and SERCOM Setup (Standard)
  GCLK->CLKCTRL.reg = GCLK_CLKCTRL_CLKEN | GCLK_CLKCTRL_GEN_GCLK4 | GCLK_CLKCTRL_ID_SERCOM1_CORE_Val;
  PM->APBCMASK.reg |= PM_APBCMASK_SERCOM1 | PM_AHBMASK_DMAC; // Enable DMAC Clock

  //Serial.println("SPI 2"); 
  
  SPI.begin();
  SERCOM1->SPI.CTRLA.reg = SERCOM_SPI_CTRLA_SWRST;
  while(SERCOM1->SPI.CTRLA.bit.SWRST);

  //Serial.println("SPI 3"); 
  
  
  // SPI master ; SPI frame format ; 
  SERCOM1->SPI.CTRLA.reg  |= SERCOM_SPI_CTRLA_MODE(SERCOM_SPI_CTRLA_MODE_SPI_MASTER_Val)
                          | SERCOM_SPI_CTRLA_FORM(0)
                          | SERCOM_SPI_CTRLA_DIPO(3)
                          | SERCOM_SPI_CTRLA_DOPO(0);
  SERCOM1->SPI.CTRLA.bit.CPOL = 0;
  SERCOM1->SPI.CTRLA.bit.CPHA = 1;
  SERCOM1->SPI.CTRLA.bit.DORD = 0;  //MSB first
 
  // Slave select low detect enable ; Master slave selection enable
  //SERCOM1->SPI.CTRLB.reg            |= SERCOM_SPI_CTRLB_MSSEN
  //                                  | SERCOM_SPI_CTRLB_SSDE;
  SERCOM1->SPI.CTRLB.bit.RXEN = 1;

  /*
  / Fix the baud rate at 1000000
  / SystemCoreClock / (2 * baudrate) - 1
  / SystemCoreClock = 8000000
  / baudrate = 1000000
  */
  SERCOM1->SPI.BAUD.bit.BAUD        = SPI_CLOCK_MHz / (2 * SPI_MHz) - 1;

  //Serial.println("SPI 3.1"); 
  // Enable SPI
  SERCOM1->SPI.CTRLA.reg            |= SERCOM_SPI_CTRLA_ENABLE;
  while(!(SERCOM1->SPI.CTRLA.reg & SERCOM_SPI_CTRLA_ENABLE)){};        // Wait until SPI is enabled


  //Serial.println("SPI 4"); 


  REG_PM_APBBMASK |= PM_APBBMASK_DMAC;           // Switch on the DMAC system peripheral
  DMAC->CTRL.bit.DMAENABLE = 0;
  //DMA_enable(0);
  DMAC->CTRL.bit.SWRST = 1;
  
  //Serial.println("DMA Setup Reseted");


  // 2. Initialize DMA Controller
  DMAC->CTRL.reg = 0; // Disable for config
   DMAC->BASEADDR.reg = (uint32_t)descriptor_section;                // Set the descriptor section base address
  DMAC->WRBADDR.reg = (uint32_t)wrb;                                // Set the write-back descriptor base adddress
  
  
  DMAC->CTRL.reg = DMAC_CTRL_DMAENABLE | DMAC_CTRL_LVLEN(0xf);      // Enable the DMAC and priority levels

 
  //Serial.println("SPI 5"); 

  setup_dma_channels();
  DMAC->CTRL.bit.DMAENABLE = 1;
}

extern "C" void setup_dma_channels()
{
  //Serial.println("SPI 10"); 
  // --- Channel 0: TX (Memory to SPI) ---
  DMAC->CHID.reg = DMAC_CHID_ID(0);
  DMAC->CHCTRLB.reg = DMAC_CHCTRLB_TRIGSRC(SERCOM1_DMAC_ID_TX) | DMAC_CHCTRLB_TRIGACT_BEAT;
  
  //Serial.println("SPI 11"); 
  descriptor.btctrl = DMAC_BTCTRL_BEATSIZE_BYTE | DMAC_BTCTRL_VALID | DMAC_BTCTRL_SRCINC; // Increment source (spi_buffer_tx)
  descriptor.btcnt = 2; // 2 bytes
  descriptor.srcaddr = (uint32_t)spi_buffer_tx + 2;
  descriptor.dstaddr = (uint32_t)&SERCOM1->SPI.DATA.reg;
  descriptor.descaddr = 0;
  memcpy(&descriptor_section[0], &descriptor, sizeof(dmacdescriptor));  // Copy to the channel 0 descriptor

  //Serial.println("SPI 12"); 
  // --- Channel 1: RX (SPI to Memory) ---
  DMAC->CHID.reg = DMAC_CHID_ID(1);
  DMAC->CHCTRLB.reg = DMAC_CHCTRLB_TRIGSRC(SERCOM1_DMAC_ID_RX) | DMAC_CHCTRLB_TRIGACT_BEAT;
  
  //descriptor.btctrl = DMAC_BTCTRL_BEATSIZE_BYTE | DMAC_BTCTRL_VALID | DMAC_BTCTRL_DSTINC | DMAC_BTCTRL_BLOCKACT_INT; // Increment dest, trigger interrupt
  descriptor.btctrl = DMAC_BTCTRL_BEATSIZE_BYTE | DMAC_BTCTRL_VALID | DMAC_BTCTRL_DSTINC; // Increment dest
  descriptor.btcnt = 2;
  descriptor.srcaddr = (uint32_t)&SERCOM1->SPI.DATA.reg;
  descriptor.dstaddr = (uint32_t)spi_buffer_rx + 2;
  descriptor.descaddr = 0;
  memcpy(&descriptor_section[1], &descriptor, sizeof(dmacdescriptor));  // Copy to the channel 1 descriptor

  //Serial.println("SPI 13"); 
  
 // Enable RX Interrupt
 // NVIC_EnableIRQ(DMAC_IRQn);
 // DMAC->CHINTENSET.reg = DMAC_CHINTENSET_TCMPL; // Transfer Complete Interrupt
 // Serial.println("SPI 14"); 
}

extern "C" uint16_t DMAreadwriteSPI(uint16_t data) {
  
  //read data from last SPI transfer
  uint16_t spi_ADCraw = ((uint16_t)spi_buffer_rx[0] << 8 | spi_buffer_rx[1]) >> ADS_bitshift;

  //prep data to send
  spi_buffer_tx[0] = data>>8; // Set first byte to transmit
  spi_buffer_tx[1] = uint8_t(data); // Set first byte to transmit

  // Enable Channels to start transfer
  DMAC->CHID.reg = DMAC_CHID_ID(1); // Enable RX first
  DMAC->CHCTRLA.reg |= DMAC_CHCTRLA_ENABLE;
  DMAC->CHID.reg = DMAC_CHID_ID(0); // Then TX
  DMAC->CHCTRLA.reg |= DMAC_CHCTRLA_ENABLE;

  return spi_ADCraw; 
}


/*
extern "C" void DMAC_Handler()
{
  uint8_t active_channel = DMAC->CHID.reg;
  DMAC->CHINTFLAG.reg = DMAC_CHINTFLAG_TCMPL; // Clear flag
  Serial.println("DMAC INT"); 

  if (active_channel == 1) { // RX Channel finished
    spi_ADCraw = ((uint16_t)spi_buffer_rx[0] << 8 | spi_buffer_rx[1]) >> ADS_bitshift;
    transfer_complete = true;
  }
}
*/




