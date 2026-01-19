#include <Arduino.h>
#include <SPI.h>
#include "stdint.h"
#include "invClockGen.h"
#include "invSPI.h"

// DMA Descriptors must be aligned to 128 bytes
__attribute__((aligned(128))) DmacDescriptor base_descriptor[3];
__attribute__((aligned(128))) DmacDescriptor wb_descriptor[3];

static uint8_t dummy_tx[2] = {0x00, 0x00};
extern "C" uint8_t spi_buffer[2] = {0, 0};
extern "C" volatile uint16_t spi_ADCraw = 0;
extern "C" volatile bool transfer_complete = false;

// Forward declaration of DMA setup
void setup_dma_channels();

extern "C" void invSPI_init(void) {
  if (invClockGen_status() == 0) {
    invClockGen_init();         
  }

  // 1. Clock and SERCOM Setup (Standard)
  GCLK->CLKCTRL.reg = GCLK_CLKCTRL_CLKEN | GCLK_CLKCTRL_GEN_GCLK4 | GCLK_CLKCTRL_ID_SERCOM1_CORE_Val;
  PM->APBCMASK.reg |= PM_APBCMASK_SERCOM1 | PM_AHBMASK_DMAC; // Enable DMAC Clock
  
  SPI.begin();
  SERCOM1->SPI.CTRLA.reg = SERCOM_SPI_CTRLA_SWRST;
  while(SERCOM1->SPI.CTRLA.bit.SWRST);
  
  SERCOM1->SPI.CTRLA.reg = SERCOM_SPI_CTRLA_MODE_SPI_MASTER | SERCOM_SPI_CTRLA_DIPO(3) | SERCOM_SPI_CTRLA_DOPO(0) | SERCOM_SPI_CTRLA_CPHA;
  SERCOM1->SPI.CTRLB.reg = SERCOM_SPI_CTRLB_RXEN;
  SERCOM1->SPI.BAUD.reg = (uint8_t)(SPI_CLOCK_MHz / (2 * SPI_MHz) - 1);
  SERCOM1->SPI.CTRLA.reg |= SERCOM_SPI_CTRLA_ENABLE;
  while(SERCOM1->SPI.CTRLA.bit.ENABLE);

  // 2. Initialize DMA Controller
  DMAC->CTRL.reg = 0; // Disable for config
  DMAC->BASEADDR.reg = (uint32_t)base_descriptor;
  DMAC->WRBADDR.reg = (uint32_t)wb_descriptor;
  DMAC->PRICTRL0.reg = DMAC_PRICTRL0_LVLPRI0(0) | DMAC_PRICTRL0_RRLVLEN0; // Round-robin priority
  DMAC->CTRL.reg = DMAC_CTRL_DMAENABLE | DMAC_CTRL_CRCENABLE;

  setup_dma_channels();
}

void setup_dma_channels() {
  // --- Channel 0: TX (Memory to SPI) ---
  DMAC->CHID.reg = DMAC_CHID_ID(0);
  DMAC->CHCTRLA.reg = DMAC_CHCTRLA_SWRST;
  DMAC->CHCTRLB.reg = DMAC_CHCTRLB_TRIGSRC(SERCOM1_DMAC_ID_TX) | DMAC_CHCTRLB_TRIGACT_BEAT;
  
  base_descriptor[0].BTCTRL.reg = DMAC_BTCTRL_VALID | DMAC_BTCTRL_SRCINC; // Increment source (dummy_tx)
  base_descriptor[0].BTCNT.reg = 2; // 2 bytes
  base_descriptor[0].SRCADDR.reg = (uint32_t)dummy_tx + 2;
  base_descriptor[0].DSTADDR.reg = (uint32_t)&SERCOM1->SPI.DATA.reg;
  base_descriptor[0].DESCADDR.reg = 0;

  // --- Channel 1: RX (SPI to Memory) ---
  DMAC->CHID.reg = DMAC_CHID_ID(1);
  DMAC->CHCTRLB.reg = DMAC_CHCTRLB_TRIGSRC(SERCOM1_DMAC_ID_RX) | DMAC_CHCTRLB_TRIGACT_BEAT;
  
  base_descriptor[1].BTCTRL.reg = DMAC_BTCTRL_VALID | DMAC_BTCTRL_DSTINC | DMAC_BTCTRL_BLOCKACT_INT; // Increment dest, trigger interrupt
  base_descriptor[1].BTCNT.reg = 2;
  base_descriptor[1].SRCADDR.reg = (uint32_t)&SERCOM1->SPI.DATA.reg;
  base_descriptor[1].DSTADDR.reg = (uint32_t)spi_buffer + 2;
  base_descriptor[1].DESCADDR.reg = 0;

  // Enable RX Interrupt
  NVIC_EnableIRQ(DMAC_IRQn);
  DMAC->CHINTENSET.reg = DMAC_CHINTENSET_TCMPL; // Transfer Complete Interrupt
}

extern "C" uint16_t readwriteSPI(uint8_t data, uint8_t byte_pos) {
  transfer_complete = false;
  dummy_tx[0] = data; // Set first byte to transmit

  // Enable Channels to start transfer
  DMAC->CHID.reg = DMAC_CHID_ID(1); // Enable RX first
  DMAC->CHCTRLA.reg |= DMAC_CHCTRLA_ENABLE;
  DMAC->CHID.reg = DMAC_CHID_ID(0); // Then TX
  DMAC->CHCTRLA.reg |= DMAC_CHCTRLA_ENABLE;

  return spi_ADCraw; 
}

void DMAC_Handler() {
  uint8_t active_channel = DMAC->CHID.reg;
  DMAC->CHINTFLAG.reg = DMAC_CHINTFLAG_TCMPL; // Clear flag

  if (active_channel == 1) { // RX Channel finished
    spi_ADCraw = ((uint16_t)spi_buffer[0] << 8 | spi_buffer[1]) >> ADS_bitshift;
    transfer_complete = true;
  }
}
