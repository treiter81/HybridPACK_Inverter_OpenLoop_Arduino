#include <Arduino.h>
#include "invADC.h"
#include "invClockGen.h"

//#define ADCdebug // can be only used with free SPI pin 13

#define invADCchannels 4
extern "C" uint16_T invADCresult[invADCchannels] = {0xFF, 0xFF, 0xFF, 0xFF};
extern "C" uint8_T invADCsequencer[invADCchannels] = {ADC_INPUTCTRL_MUXPOS_PIN17, ADC_INPUTCTRL_MUXPOS_PIN18, ADC_INPUTCTRL_MUXPOS_PIN0, ADC_INPUTCTRL_MUXPOS_PIN1};

extern "C" uint8_T invADCchannel = 0;
extern "C" uint16_T invADCcounter = 0;

extern "C" uint8_T invADCinitdone = 0;

extern "C" void invADC_init(void) //ADC init with Clock3
{
  if (invADCinitdone == 0)          //if ADC not inizialized yet
  {
  
  if (invClockGen_status() == 0)    //if generic clocks are not running; start them
  {
    invClockGen_init();         
  }

  #ifdef ADCdebug
    pinMode(13, OUTPUT);
  #endif
    
  /* Enable the APB clock for the ADC. */
  PM->APBCMASK.reg |= PM_APBCMASK_ADC;            //activate ADC peripheral
  PM->APBCMASK.reg |= PM_APBCMASK_EVSYS;          //activate Eventsystem peripheral
  

  REG_GCLK_CLKCTRL = GCLK_CLKCTRL_CLKEN |         // Enable GCLK0 as a clock source (1 << 14)
                       GCLK_CLKCTRL_GEN_GCLK3 |     // Select GCLK3 at 8MHz (0 << 8)
                       GCLK_CLKCTRL_ID_ADC;   // Route GCLK6 to ADC
  while (GCLK->STATUS.bit.SYNCBUSY);              // Wait for synchronization

  ADC->CTRLA.bit.SWRST = 0x01;                    // RESET the ADC
  while(ADC->STATUS.bit.SYNCBUSY);                 // Wait for synchronization

  ADC->CTRLB.reg = ADC_CTRLB_PRESCALER_DIV4 |    // Divide Clock ADC GCLK by 4 (8MHz/2 = 2000 kHz)
                    ADC_CTRLB_RESSEL_12BIT;         // Set ADC resolution to 12 bits
  while(ADC->STATUS.bit.SYNCBUSY);                 // Wait for synchronization


  ADC->SAMPCTRL.reg = 0x01;                        // Set max Sampling Time Length to half divided ADC clock pulse (5.33us)
  while(ADC->STATUS.bit.SYNCBUSY);                 // Wait for synchronization

  ADC->AVGCTRL.reg = 0;                           // no averaging
  while(ADC->STATUS.bit.SYNCBUSY);                 // Wait for synchronization

  //ADC->REFCTRL.reg = ADC_REFCTRL_REFCOMP |
  //                    ADC_REFCTRL_REFSEL_INTVCC1;
  while(ADC->STATUS.bit.SYNCBUSY);                 // Wait for synchronization
  

  ADC->REFCTRL.bit.REFCOMP = 1;
      while(ADC->STATUS.bit.SYNCBUSY);                 // Wait for synchronization
   ADC->REFCTRL.bit.REFSEL = 3;
      while(ADC->STATUS.bit.SYNCBUSY);                 // Wait for synchronization


  
  
  ADC->INPUTCTRL.reg = ADC_INPUTCTRL_GAIN_DIV2 |
                     ADC_INPUTCTRL_MUXNEG_GND |
                     ADC_INPUTCTRL_MUXPOS_PIN17;
  while(ADC->STATUS.bit.SYNCBUSY);                 // Wait for synchronization


  ADC->AVGCTRL.reg = ADC_AVGCTRL_SAMPLENUM_1;
  while(ADC->STATUS.bit.SYNCBUSY);                 // Wait for synchronization
 
  ADC->CTRLA.bit.ENABLE = 0x01;                    // Enable the ADC
  while(ADC->STATUS.bit.SYNCBUSY);                 // Wait for synchronization


  //************************************** Load ADC Calibration Data ********************************************************//
  uint32_t bias = (*((uint32_t *) ADC_FUSES_BIASCAL_ADDR) & ADC_FUSES_BIASCAL_Msk) >> ADC_FUSES_BIASCAL_Pos;
  uint32_t linearity = (*((uint32_t *) ADC_FUSES_LINEARITY_0_ADDR) & ADC_FUSES_LINEARITY_0_Msk) >> ADC_FUSES_LINEARITY_0_Pos;
  linearity |= ((*((uint32_t *) ADC_FUSES_LINEARITY_1_ADDR) & ADC_FUSES_LINEARITY_1_Msk) >> ADC_FUSES_LINEARITY_1_Pos) << 5;
  while(ADC->STATUS.bit.SYNCBUSY);                 // Wait for synchronization

  /* Write the calibration data. */
  ADC->CALIB.reg = ADC_CALIB_BIAS_CAL(bias) | ADC_CALIB_LINEARITY_CAL(linearity);
  while(ADC->STATUS.bit.SYNCBUSY);                 // Wait for synchronization




//************************************** Through away 1st ADC result ********************************************************//
  ADC->SWTRIG.bit.START = 1;                       // Initiate a software trigger to start an ADC conversion
  while(ADC->STATUS.bit.SYNCBUSY);                 // Wait for write synchronization

  while(!ADC->INTFLAG.bit.RESRDY);                 // Wait for the conversion to complete
  ADC->INTFLAG.bit.RESRDY = 1;                     // Clear the result ready (RESRDY) interrupt flag


//************************************** Enable the TCC1 Interrupts ********************************************************//


  EVSYS->USER.reg = EVSYS_USER_CHANNEL(1) |                                // Attach the event user (receiver) to channel 2 (n + 1)
                    EVSYS_USER_USER(EVSYS_ID_USER_ADC_START);              // Set the event user (receiver) as ADC start

  EVSYS->CHANNEL.reg = EVSYS_CHANNEL_EDGSEL_NO_EVT_OUTPUT |                // No event edge detection
                       EVSYS_CHANNEL_PATH_ASYNCHRONOUS |                   // Set event path as asynchronous
                       EVSYS_CHANNEL_EVGEN(EVSYS_ID_GEN_TCC0_OVF) |        // Set event generator (sender) as TCC0_OVF
                       EVSYS_CHANNEL_CHANNEL(0);                           // Attach the generator (sender) to channel 0

  //TCC0->EVCTRL.reg = TCC_EVCTRL_OVFEO;                                  //placed at PWM init. Set before PWM started
  ADC->EVCTRL.reg |= ADC_EVCTRL_STARTEI;                                  //activate ADC conversion start at incomming events
  
  //************************************** Enable the ADC Ready Interrupt ********************************************************//
  ADC->INTENSET.reg = 0x1;
  while(ADC->STATUS.bit.SYNCBUSY);                 // Wait for synchronization

  NVIC_SetPriority(ADC_IRQn,3);
  NVIC_EnableIRQ(ADC_IRQn);

  invADCinitdone = 1;
  Serial.println("ADC Setup done");
  }
  else{Serial.println("ADC already inizialized");}

}


extern "C" void invADC_usertrigger(void) 
{
    if ((invADCinitdone == 1)&&(TCC0->CTRLA.bit.ENABLE == 0))
    {
      ADC->SWTRIG.bit.START = 1;                       // Initiate a software trigger to start an ADC conversion
      while(ADC->STATUS.bit.SYNCBUSY);                 // Wait for write synchronization       
    }
}


extern "C" uint16_T invADC_read(uint8_T channel) 
{
  return (invADCresult[channel]);
}


extern "C" uint16_T invADC_readcounter(void) 
{
  return (invADCcounter);
}




extern "C" void ADC_Handler(void)              //read ADC result when ready and switches to the next sequencer channel
{
  if (ADC->INTFLAG.bit.RESRDY == 1)
  {
    ADC->INTFLAG.bit.RESRDY = 1;
    while(ADC->STATUS.bit.SYNCBUSY);
    
    invADCresult[invADCchannel] = ADC->RESULT.reg;            //store ADC result
    invADCcounter++;                                          //next channel
    invADCchannel = invADCcounter & 0x0003;                   //use only last bits avoid if else for speed

    ADC->INPUTCTRL.bit.MUXPOS = invADCsequencer[invADCchannel];// Set the analog input to next input pin
    while(ADC->STATUS.bit.SYNCBUSY);                          // Wait for synchronization
  }
}
