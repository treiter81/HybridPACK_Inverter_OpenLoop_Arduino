#include <Arduino.h>
#include <SPI.h>
#include "stdint.h"
#include "invClockGen.h"
#include "invSPI.h"


extern "C" volatile uint8_t spi_byte_pos = 0;
extern "C" volatile uint8_t spi_buffer[2] = {0xF,0xF};
extern "C" volatile uint16_t spi_ADCraw = 0;


extern "C" void invSPI_init(void) { //SPI init

  if (invClockGen_status() == 0)    //if generic clocks are not running; start them
  {
    invClockGen_init();         
  }

  //init the clock on sercom device
  GCLK->CLKCTRL.reg = GCLK_CLKCTRL_CLKEN |              // Select GCLK4 as a clock source for SERCOM1
                      GCLK_CLKCTRL_GEN_GCLK4 |          // Select GCLK4
                      GCLK_CLKCTRL_ID_SERCOM1_CORE_Val; // Select GCLK4 for SERCOM1
  // Enable SERCOM 0
  PM->APBCMASK.reg             |= PM_APBCMASK_SERCOM1;

  //CS is done from PWM
  //pinMode(pin_CS, OUTPUT);
  
  SPI.begin();    //to set also the ports

  // Software reset
  SERCOM1->SPI.CTRLA.reg            |= SERCOM_SPI_CTRLA_SWRST;
  while(SERCOM1->SPI.CTRLA.bit.SWRST){};                    // Wait until reset
  
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
 

  // Enable SPI
  SERCOM1->SPI.CTRLA.reg            |= SERCOM_SPI_CTRLA_ENABLE;
  while(!(SERCOM1->SPI.CTRLA.reg & SERCOM_SPI_CTRLA_ENABLE)){};        // Wait until SPI is enabled

  //NVIC_DisableIRQ(EIC_IRQn);        //disable the external interrupts
  //NVIC_ClearPendingIRQ(EIC_IRQn);        
  //NVIC_SetPriority(EIC_IRQn,1);
  //NVIC_EnableIRQ(EIC_IRQn);          //enable external interrupts again
 
  NVIC_SetPriority(SERCOM1_IRQn, 0);                                   // Set the interrupt priority to 0 (highest value)
  NVIC_EnableIRQ(SERCOM1_IRQn);                                        // Enable the interrupt

  SERCOM1->SPI.INTENSET.bit.RXC = 1;    // Enable Receive interrupts
  

  Serial.println("SPI init done");  
}


extern "C" uint16_t readwriteSPI(uint8_t data,uint8_t byte_pos)
{ 

  SERCOM1->SPI.DATA.bit.DATA = data; // Writing data into Data register
  spi_byte_pos = byte_pos;
  
  return (spi_ADCraw);
}




extern "C" void SERCOM1_Handler(void)              //PWM Interrupt Routine
{
  if (SERCOM1->SPI.INTFLAG.bit.RXC == 1) 
  {
    //SERCOM1->SPI.INTFLAG.bit.RXC = 1;     //clear interrupt flag
    if (spi_byte_pos==1)
    {
      spi_buffer[0] = SERCOM1->SPI.DATA.bit.DATA;
      spi_byte_pos = 0;
      while(SERCOM1->SPI.INTFLAG.bit.DRE == 0){}; //wait until ready for new data
      readwriteSPI(0,2);      
    }
    else
    {
      spi_buffer[1] = SERCOM1->SPI.DATA.bit.DATA;
      //digitalWrite(pin_CS, HIGH);
      spi_byte_pos = 0;

      /*
      //#ifdef DEBUG
        Serial.print("Receive Interrupt data 1: "); 
        Serial.println(spi_buffer[0]); 
        Serial.print("Receive Interrupt data 2: "); 
        Serial.println(spi_buffer[1]); 
        Serial.println("");
      //#endif
       */

      spi_ADCraw = (spi_buffer[0]<<8 | spi_buffer[1])>>ADS_bitshift;
      
      /*
      Serial.println();
      Serial.print("ADC result: ");
      Serial.println(spi_ADCraw);
      */
     
    }
  }         
}
