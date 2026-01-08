#include <Arduino.h>
#include "invCAPTURE.h"
#include "invClockGen.h"

#define Pin_DATA5 15
#define Pin_DATA6 7

extern "C" uint16_T invCAPTUREinitdone = 0;

extern "C" uint16_T Data5_PulseW = 0;
extern "C" uint16_T Data6_PulseW = 0;

extern "C" uint16_T invTempsense[2] = {0, 0};


extern "C" void invCapture_init(void) //ADC init with Clock6
{
  if (invClockGen_status() == 0)    //if generic clocks are not running; start them
  {
    invClockGen_init();         
  }

  PM->APBCMASK.reg |= PM_APBCMASK_EVSYS;     // Switch on the event system peripheral
  
  // Enable GCLK5 and connect it to TC4 and TC5
  GCLK->CLKCTRL.reg = GCLK_CLKCTRL_CLKEN |        // Enable generic clock
                       GCLK_CLKCTRL_GEN_GCLK5 |    // Select GCLK5
                       GCLK_CLKCTRL_ID_TC4_TC5;
  while (GCLK->STATUS.bit.SYNCBUSY); 


  ///////////////////////////////////////////////////
  // Timer input on DATA5, with timer T4
  ///////////////////////////////////////////////////
  
  pinMode(Pin_DATA5, INPUT);
  // Enable the port multiplexer on digital pin D12
  PORT->Group[g_APinDescription[Pin_DATA5].ulPort].PINCFG[g_APinDescription[Pin_DATA5].ulPin].bit.PMUXEN = 1;
   // Set-up the pin as an EIC (interrupt) peripheral on digital pin D12
  PORT->Group[g_APinDescription[Pin_DATA5].ulPort].PMUX[g_APinDescription[Pin_DATA5].ulPin >> 1].reg |= PORT_PMUX_PMUXE_A;
 
  
 
  EIC->EVCTRL.reg |= EIC_EVCTRL_EXTINTEO2;          // Enable event output on external interrupt 2
  EIC->CONFIG[0].reg |= EIC_CONFIG_SENSE2_HIGH;     // Set event detecting a HIGH level
  //EIC->CONFIG[0].reg |= EIC_CONFIG_FILTEN2;         // set majority filter to cancel noise
  EIC->INTENCLR.reg = EIC_INTENCLR_EXTINT2;         // Disable interrupts on external interrupt 2
  EIC->CTRL.bit.ENABLE = 1;                         // Enable EIC peripheral
  while (EIC->STATUS.bit.SYNCBUSY);                 // Wait for synchronization

  EVSYS->USER.reg = EVSYS_USER_CHANNEL(3) |                                // Attach the event user (receiver) to channel 2 (n + 1)
                    EVSYS_USER_USER(EVSYS_ID_USER_TC4_EVU);                // Set the event user (receiver) as timer TC4

  EVSYS->CHANNEL.reg = EVSYS_CHANNEL_EDGSEL_NO_EVT_OUTPUT |                // No event edge detection
                       EVSYS_CHANNEL_PATH_ASYNCHRONOUS |                   // Set event path as asynchronous
                       EVSYS_CHANNEL_EVGEN(EVSYS_ID_GEN_EIC_EXTINT_2) |    // Set event generator (sender) as external interrupt 3
                       EVSYS_CHANNEL_CHANNEL(2);                           // Attach the generator (sender) to channel 2



  TC4->COUNT16.EVCTRL.reg = TC_EVCTRL_TCEI |              // Enable the TC event input
                            /*TC_EVCTRL_TCINV |*/         // Invert the event input
                            TC_EVCTRL_EVACT_PPW;          // Set up the timer for capture: CC0 period, CC1 pulsewidth
  
  TC4->COUNT16.CTRLC.reg = TC_CTRLC_CPTEN1 |              // Enable capture on CC1
                           TC_CTRLC_CPTEN0;               // Enable capture on CC0
  while (TC4->COUNT16.STATUS.bit.SYNCBUSY);               // Wait for synchronization

  NVIC_SetPriority(TC4_IRQn, 0);      // Set the Nested Vector Interrupt Controller (NVIC) priority for TC4 to 0 (highest)
  NVIC_EnableIRQ(TC4_IRQn);           // Connect the TC3 timer to the Nested Vector Interrupt Controller (NVIC)

  //TC4->COUNT16.INTENSET.reg = TC_INTENSET_MC1 |           // Enable compare channel 1 (CC1) interrupts
  //                            TC_INTENSET_MC0;            // Enable compare channel 0 (CC0) interrupts
  TC4->COUNT16.INTENSET.reg = TC_INTENSET_MC1;              // Enable compare channel 1 (CC1) interrupts

  TC4->COUNT16.CTRLA.reg = TC_CTRLA_PRESCSYNC_PRESC |     // Set timer to overflow on prescaler clock (rather than GCLK)
                           TC_CTRLA_PRESCALER_DIV1;      // Set prescaler to 1
                 
  TC4->COUNT16.CTRLA.bit.ENABLE = 1;                      // Enable the TC4 timer
  while (TC4->COUNT16.STATUS.bit.SYNCBUSY);               // Wait for synchronization
  
  TC4->COUNT16.READREQ.reg = TC_READREQ_RREQ |      // Enable a read request
                             TC_READREQ_RCONT |     // continous sync enabled
                             TC_READREQ_ADDR(0x1A); // Offset address of the CC1 register
  while (TC4->COUNT16.STATUS.bit.SYNCBUSY);         // Wait for (read) synchronization



///////////////////////////////////////////////////
  // Timer input on DATA6, with timer T5
  ///////////////////////////////////////////////////
  pinMode(Pin_DATA6, INPUT);
  // Enable the port multiplexer on digital pin D7
  PORT->Group[g_APinDescription[Pin_DATA6].ulPort].PINCFG[g_APinDescription[Pin_DATA6].ulPin].bit.PMUXEN = 1;
   // Set-up the pin as an EIC (interrupt) peripheral on digital pin D7
  PORT->Group[g_APinDescription[Pin_DATA6].ulPort].PMUX[g_APinDescription[Pin_DATA6].ulPin >> 1].reg |= PORT_PMUX_PMUXE_A;

  
 
  EIC->EVCTRL.reg |= EIC_EVCTRL_EXTINTEO6;          // Enable event output on external interrupt 6
  EIC->CONFIG[0].reg |= EIC_CONFIG_SENSE6_HIGH;     // Set event detecting a HIGH level
  //EIC->CONFIG[0].reg |= EIC_CONFIG_SENSE6_HIGH | EIC_CONFIG_FILTEN6;     // Set event detecting a HIGH level and set majority filter to cancel noise
  EIC->INTENCLR.reg = EIC_INTENCLR_EXTINT6;         // Disable interrupts on external interrupt 6
  EIC->CTRL.bit.ENABLE = 1;                         // Enable EIC peripheral
  while (EIC->STATUS.bit.SYNCBUSY);                 // Wait for synchronization

  EVSYS->USER.reg = EVSYS_USER_CHANNEL(5) |                                // Attach the event user (receiver) to channel 4 (n + 1)
                    EVSYS_USER_USER(EVSYS_ID_USER_TC5_EVU);                // Set the event user (receiver) as timer TC5

  EVSYS->CHANNEL.reg = EVSYS_CHANNEL_EDGSEL_NO_EVT_OUTPUT |                // No event edge detection
                       EVSYS_CHANNEL_PATH_ASYNCHRONOUS |                   // Set event path as asynchronous
                       EVSYS_CHANNEL_EVGEN(EVSYS_ID_GEN_EIC_EXTINT_6) |    // Set event generator (sender) as external interrupt 3
                       EVSYS_CHANNEL_CHANNEL(4);                           // Attach the generator (sender) to channel 4



  TC5->COUNT16.EVCTRL.reg = TC_EVCTRL_TCEI |              // Enable the TC event input
                            /*TC_EVCTRL_TCINV |*/         // Invert the event input
                            TC_EVCTRL_EVACT_PPW;          // Set up the timer for capture: CC0 period, CC1 pulsewidth
  
  TC5->COUNT16.CTRLC.reg = TC_CTRLC_CPTEN1 |              // Enable capture on CC1
                           TC_CTRLC_CPTEN0;               // Enable capture on CC0
  while (TC5->COUNT16.STATUS.bit.SYNCBUSY);               // Wait for synchronization

  NVIC_SetPriority(TC5_IRQn, 2);      // Set the Nested Vector Interrupt Controller (NVIC) priority for TC3 to 1 (highest)
  NVIC_EnableIRQ(TC5_IRQn);           // Connect the TC3 timer to the Nested Vector Interrupt Controller (NVIC)

  //TC5->COUNT16.INTENSET.reg = TC_INTENSET_MC1 |           // Enable compare channel 1 (CC1) interrupts
  //                            TC_INTENSET_MC0;            // Enable compare channel 0 (CC0) interrupts
  TC5->COUNT16.INTENSET.reg = TC_INTENSET_MC1;              // Enable compare channel 1 (CC1) interrupts

  TC5->COUNT16.CTRLA.reg = TC_CTRLA_PRESCSYNC_PRESC |     // Set timer to overflow on prescaler clock (rather than GCLK)
                           TC_CTRLA_PRESCALER_DIV1;      // Set prescaler to 1
                 
  TC5->COUNT16.CTRLA.bit.ENABLE = 1;                      // Enable the TC5 timer
  while (TC5->COUNT16.STATUS.bit.SYNCBUSY);               // Wait for synchronization
  
  TC5->COUNT16.READREQ.reg = TC_READREQ_RREQ |      // Enable a read request
                             TC_READREQ_RCONT |     // continous sync enabled
                             TC_READREQ_ADDR(0x1A); // Offset address of the CC1 register
  while (TC5->COUNT16.STATUS.bit.SYNCBUSY);         // Wait for (read) synchronization
  
  invCAPTUREinitdone = 1;
  Serial.println("TC Capture init done");  
 
}


extern "C" void TC4_Handler(void)                                    // Interrupt Service Routine (ISR) for timer TC4
{  
  if (TC4->COUNT16.INTFLAG.bit.MC1)                   // Check for match counter 1 (MC1) interrupt
  {
    //todo clear intflag

    Data5_PulseW = TC4->COUNT16.CC[1].reg;           // Copy the pulse-width
    if (Data5_PulseW < 10000)
    {
      invTempsense[0] = Data5_PulseW;
    }
  }
}


extern "C" void TC5_Handler(void)                                    // Interrupt Service Routine (ISR) for timer TC4
{
 
  if (TC5->COUNT16.INTFLAG.bit.MC1)                   // Check for match counter 1 (MC1) interrupt
  {
    //todo clear intflag

    Data6_PulseW = TC5->COUNT16.CC[1].reg;           // Copy the pulse-width
    if (Data6_PulseW < 10000)
    {
      invTempsense[1] = Data6_PulseW;
    }
  }
}


extern "C" uint16_T invCapture_readData5(void) 
{
  return (invTempsense[0]);
}


extern "C" uint16_T invCapture_readData6(void) 
{
  return (invTempsense[1]);
}

