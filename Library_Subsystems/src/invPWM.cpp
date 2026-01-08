#include <Arduino.h>
#include "invPWM.h"
#include "invClockGen.h"

//#define debug

extern "C" uint8_T invPWMinitdone = 0;
extern "C" uint16_T invPWMperiod = 5000;


extern "C" void invPWM_init(void) //PWM init with TCC0 as counter
{
  if (invPWMinitdone == 0)
  {
  if (invClockGen_status() == 0)    //if generic clocks are not running; start them
  {
    invClockGen_init();         
  }
  
  PM->APBCMASK.reg |= PM_APBCMASK_EVSYS;          //activate eventsystem

  // Enable GCLK5 and connect it to TCC0 and TCC1
  GCLK->CLKCTRL.reg = GCLK_CLKCTRL_CLKEN |        // Enable generic clock
                      GCLK_CLKCTRL_GEN_GCLK5 |    // Select GCLK5
                      GCLK_CLKCTRL_ID_TCC0_TCC1;   // Route GCLK5 to TCC0 and TCC1 (0x1A << 0)
  while (GCLK->STATUS.bit.SYNCBUSY);              // Wait for synchronization



  //************************************** PORT Enabling ********************************************************//
  
   pinMode(pin_EN, OUTPUT);
   //digitalWrite(pin_EN, LOW);                     // Driver Disable
   REG_PORT_DIRSET0 = (1ul << pin_EN);
   REG_PORT_OUTCLR0 = (1ul << pin_EN);

  //D6 = PWM Lowside U
  //D2 = PWM Highside U
    // Enable the port multiplexer for pins D6
    PORT->Group[g_APinDescription[pin_LSU].ulPort].PINCFG[g_APinDescription[pin_LSU].ulPin].bit.PMUXEN = 1;
    // Enable the port multiplexer for pins D2
    PORT->Group[g_APinDescription[pin_HSU].ulPort].PINCFG[g_APinDescription[pin_HSU].ulPin].bit.PMUXEN = 1;

    // D6 is on EVEN port pin PA04 and TCC0/WO[0] channel 0 is on peripheral E
    PORT->Group[g_APinDescription[pin_LSU].ulPort].PMUX[g_APinDescription[pin_LSU].ulPin >> 1].reg |= PORT_PMUX_PMUXE_E;
    // D2 is on EVEN port pin PB10 and TCC0/WO[4] channel 0 is on peripheral F
    PORT->Group[g_APinDescription[pin_HSU].ulPort].PMUX[g_APinDescription[pin_HSU].ulPin >> 1].reg |= PORT_PMUX_PMUXE_F;
  

  //D8 = PWM Lowside V
  //D9 = PWM Highside V
    // Enable the port multiplexer for pins D8
    PORT->Group[g_APinDescription[pin_LSV].ulPort].PINCFG[g_APinDescription[pin_LSV].ulPin].bit.PMUXEN = 1;
    // Enable the port multiplexer for pins D9
    PORT->Group[g_APinDescription[pin_HSV].ulPort].PINCFG[g_APinDescription[pin_HSV].ulPin].bit.PMUXEN = 1;

    // D8 is on EVEN port pin PA18 and TCC0/WO[2] channel 0 is on peripheral F
    PORT->Group[g_APinDescription[pin_LSV].ulPort].PMUX[g_APinDescription[pin_LSV].ulPin >> 1].reg |= PORT_PMUX_PMUXE_F;
    // D9 is on UNEVEN port pin PA20 and TCC0/WO[6] channel 0 is on peripheral F
    PORT->Group[g_APinDescription[pin_HSV].ulPort].PMUX[g_APinDescription[pin_HSV].ulPin >> 1].reg |= PORT_PMUX_PMUXE_F;


  //D16 = PWM Lowside W
  //D10 = PWM Highside W
    // Enable the port multiplexer for pins D16
    PORT->Group[g_APinDescription[pin_LSW].ulPort].PINCFG[g_APinDescription[pin_LSW].ulPin].bit.PMUXEN = 1;
    // Enable the port multiplexer for pins D10
    PORT->Group[g_APinDescription[pin_HSW].ulPort].PINCFG[g_APinDescription[pin_HSW].ulPin].bit.PMUXEN = 1;

    // D16 is on UNEVEN port pin PA11 and TCC0/WO[2] channel 0 is on peripheral F
    PORT->Group[g_APinDescription[pin_LSW].ulPort].PMUX[g_APinDescription[pin_LSW].ulPin >> 1].reg |= PORT_PMUX_PMUXO_F;
    // D10 is on UNEVEN port pin PA21 and TCC0/WO[6] channel 0 is on peripheral F
    PORT->Group[g_APinDescription[pin_HSW].ulPort].PMUX[g_APinDescription[pin_HSW].ulPin >> 1].reg |= PORT_PMUX_PMUXO_F;
    
 
   //D4 = PWM Chipselect and IRQ Trigger for the control loops
    // Enable the port multiplexer for pins D4
    PORT->Group[g_APinDescription[pin_CS].ulPort].PINCFG[g_APinDescription[pin_CS].ulPin].bit.PMUXEN = 1;
    // D4 is on EVEN port pin PA07 and TCC0/WO[0] channel 0 is on peripheral E
     PORT->Group[g_APinDescription[pin_CS].ulPort].PMUX[g_APinDescription[pin_CS].ulPin >> 1].reg |= PORT_PMUX_PMUXO_E;

  //************************************** PORT Enabling ********************************************************//


   

  //************************************** TIMER Setting ********************************************************//
    REG_TCC0_WAVE = TCC_WAVE_WAVEGEN_DSBOTH;          // Dual slope PWM on TCC0  with Interrupt at Zero and Top
    while (TCC0->SYNCBUSY.bit.WAVE);                 // Wait for synchronization

    TCC0->EVCTRL.reg |= TCC_EVCTRL_OVFEO;           //Activate the Events at Overflow Zero and Top. Can be used to trigger ADCs


    REG_TCC1_WAVE = TCC_WAVE_WAVEGEN_NPWM;          // Setup single slope PWM on TCC1 
    while (TCC1->SYNCBUSY.bit.WAVE);                 // Wait for synchronization

    //TCC1->EVCTRL.reg = TCC_EVCTRL_OVFEO;            //Activate the Events at Overflow. Can be used to trigger DMA/SPI

    REG_TCC0_CTRLA |= TCC_CTRLA_PRESCALER_DIV1 ;      // Set prescaler to 1, 100MHz/1 (0 << 8)
    REG_TCC1_CTRLA |= TCC_CTRLA_PRESCALER_DIV1 ;      // Set prescaler to 1, 100MHz/1 (0 << 8)

    
    REG_TCC0_PER = invPWMperiod;                       // Set the frequency of the PWM on TCC0 
    while (TCC0->SYNCBUSY.bit.PER);                  // Wait for synchronization

    REG_TCC1_PER = (invPWMperiod<<1)-1;                // Set the frequency on the PWM on TCC1 
    while (TCC1->SYNCBUSY.bit.PER);                  // Wait for synchronization


    REG_TCC0_CC0 = invPWMperiod>>1;                           // CC0 - 50% duty cycle Phase U
    while (TCC0->SYNCBUSY.bit.CC0);                    // Wait for synchronization
    REG_TCC0_CC2 = invPWMperiod>>1;                           // CC0 - 50% duty cycle Phase V
    while (TCC0->SYNCBUSY.bit.CC2);                    // Wait for synchronization
    REG_TCC0_CC3 = invPWMperiod>>1;                           // CC0 - 50% duty cycle Phase W
    while (TCC0->SYNCBUSY.bit.CC3);                    // Wait for synchronization

    REG_TCC1_CC1 = (invPWMperiod<<1)-100;                    // CC1 - 50% duty cycle Phase W
    while (TCC1->SYNCBUSY.bit.CC1);                    // Wait for synchronization

    TCC1->DRVCTRL.bit.INVEN1 = 1;

    //TCC0->CTRLBSET.bit.CMD = 0x3;                   // Force Update of Double Buffer Register
    //TCC1->CTRLBSET.bit.CMD = 0x3;                   // Force Update of Double Buffer Register

    uint8_t deadtime_init = 50;
    // dead time generation 
    REG_TCC0_WEXCTRL = (deadtime_init << 16) | (deadtime_init << 24) | (15 << 8) | (0 << 0);   
    //Lowside Deadtime (bit16) <<16; Highside (bit24) <<24
    //insert deadtime on which channels (all << 8)
    //0<<0 use CC0 for Phase U; use CC2 for Phase V; use CC3 for Phase W
    
    //ensure low level on all channel in case PWM is disabled
    TCC0->DRVCTRL.bit.NRE0 = 1;
    TCC0->DRVCTRL.bit.NRE1 = 1;
    TCC0->DRVCTRL.bit.NRE2 = 1;
    TCC0->DRVCTRL.bit.NRE3 = 1;
    TCC0->DRVCTRL.bit.NRE4 = 1;
    TCC0->DRVCTRL.bit.NRE5 = 1;
    TCC0->DRVCTRL.bit.NRE6 = 1;
    TCC0->DRVCTRL.bit.NRE7 = 1;

    TCC0->CTRLBSET.reg = TCC_CTRLBSET_CMD_READSYNC;     //phase sync the TCC0 PWM with the TCC1 Trigger
    while (TCC0->SYNCBUSY.bit.COUNT);
    TCC1->COUNT.reg = (TCC0->COUNT.reg);


    //at init disable PWM Output as default
    TCC0->CTRLA.bit.ENABLE = 0;                     // disable the PWM TCC0 counter
    while (TCC0->SYNCBUSY.bit.ENABLE);              // Wait for synchronization


    TCC1->CTRLA.bit.ENABLE = 1;                     // enable the PWM TCC1 counter
    while (TCC0->SYNCBUSY.bit.ENABLE);              // Wait for synchronization

    invPWMinitdone = 1;
    Serial.println("PWM Setup done");
    }
 }



extern "C" void invPWM_start(void)
{
    TCC0->CTRLBSET.bit.CMD = 0x3;                   // Force Update of Double Buffer Register
    TCC0->CTRLA.bit.ENABLE = 1;                     // enable [1]/disable[0] the PWM TCC0 counter
    while (TCC0->SYNCBUSY.bit.ENABLE);              // Wait for synchronization
    
    
    TCC0->CTRLBSET.reg = TCC_CTRLBSET_CMD_READSYNC;     //phase sync the TCC0 PWM with the TCC1 Trigger
    while (TCC0->SYNCBUSY.bit.COUNT);
    TCC1->COUNT.reg = (TCC0->COUNT.reg);
      
    #ifdef debug
        Serial.println("PWM Start");
        Serial.println("PWM Period");
        Serial.println(REG_TCC0_PER);    
        Serial.println(REG_TCC1_PER);
        Serial.println(REG_TCC1_CC1);
    #endif
}

extern "C" void invPWM_stop(void)
{    
    TCC0->CTRLA.bit.ENABLE = 0;                     // enable [1]/disable[0] the PWM TCC0 counter
    while (TCC0->SYNCBUSY.bit.ENABLE);              // Wait for synchronization 
    #ifdef debug
        Serial.println("PWM Stop");
    #endif
}



extern "C" void invPWM_dutycycle(uint16_T period, uint16_T Du, uint16_T Dv, uint16_T Dw) //Duty Cycle: DutyCycle[3] = {D_phU,D_phV,D_phW};
{
  REG_TCC0_CCB0 = Du;
  while (TCC0->SYNCBUSY.bit.CCB0);  

  REG_TCC0_CCB2 = Dv;
  while (TCC0->SYNCBUSY.bit.CCB2);  
  
  REG_TCC0_CCB3 = Dw;
  while (TCC0->SYNCBUSY.bit.CCB3);  

  if (invPWMperiod != period)                         //Update Period only when necessary
  {
      invPWMperiod = period;
      
      REG_TCC0_PERB = invPWMperiod;                   // Set the frequency of the PWM on TCC0 
      while (TCC0->SYNCBUSY.bit.PERB);                // Wait for synchronization
      TCC0->CTRLBSET.bit.CMD = 0x3;                   // Force Update of Double Buffer Register


      if (invPWMperiod>2000) //normal case for 2 to 25kHz. Control task each PWM cycle
      {
        REG_TCC1_PERB = (invPWMperiod<<1)-1;                     // Set the frequency on the PWM on TCC1 
        while (TCC1->SYNCBUSY.bit.PERB);                  // Wait for synchronization
    
        REG_TCC1_CCB1 = (invPWMperiod<<1)-100;                    // CC1 - 50% duty cycle Phase W
        while (TCC1->SYNCBUSY.bit.CCB1);                    // Wait for synchronization
        TCC1->CTRLBSET.bit.CMD = 0x3;                   // Force Update of Double Buffer Register
      }
      else //if PWM frequency more than 25kHz do Control/ADC only each 2nd period
      {
        REG_TCC1_PERB = (invPWMperiod<<2)-1;                     // Set the frequency on the PWM on TCC1 
        while (TCC1->SYNCBUSY.bit.PERB);                  // Wait for synchronization
    
        REG_TCC1_CCB1 = (invPWMperiod<<2)-100;                    // CC1 - 50% duty cycle Phase W
        while (TCC1->SYNCBUSY.bit.CCB1);                    // Wait for synchronization
        TCC1->CTRLBSET.bit.CMD = 0x3;                   // Force Update of Double Buffer Register
      }
          
        
      TCC0->CTRLBSET.reg = TCC_CTRLBSET_CMD_READSYNC;     //phase sync the TCC0 PWM with the TCC1 Trigger
      while (TCC0->SYNCBUSY.bit.COUNT);
      TCC1->COUNT.reg = (TCC0->COUNT.reg);

      #ifdef debug
        Serial.println("PWM Frequency Changed");        
        Serial.println(period);
      #endif                    
   
  }    
}

extern "C" uint8_T invPWM_status(void)
{
    return (TCC0->CTRLA.bit.ENABLE);
}


extern "C" void invPWM_deadtime(uint8_T deadtime)
{
  if (deadtime != (REG_TCC0_WEXCTRL >> 24))       //Execute only if dead time changes
  {
    if (TCC0->CTRLA.bit.ENABLE == 1)              //if PWM running: stop, change deadtime and restart
    {
        TCC0->CTRLA.bit.ENABLE = 0;                     // Enable the TCC0 counter
        while (TCC0->SYNCBUSY.bit.ENABLE);              // Wait for synchronization  
        REG_TCC0_WEXCTRL = (deadtime << 16) | (deadtime << 24) | (15 << 8) | (0 << 0);  // (1 << 8): implement dead time at output WO[0] and WO[4]
        TCC0->CTRLA.bit.ENABLE = 1;                     // Enable the TCC0 counter
        while (TCC0->SYNCBUSY.bit.ENABLE);              // Wait for synchronization

        TCC0->CTRLBSET.reg = TCC_CTRLBSET_CMD_READSYNC;     //phase sync the TCC0 PWM with the TCC1 Trigger
        while (TCC0->SYNCBUSY.bit.COUNT);
        TCC1->COUNT.reg = (TCC0->COUNT.reg);

        #ifdef debug
            Serial.println("PWM Deadtime Changed during runtime");
        #endif
    }
    else
    {
        REG_TCC0_WEXCTRL = (deadtime << 16) | (deadtime << 24) | (15 << 8) | (0 << 0);  // (1 << 8): implement dead time at output WO[0] and WO[4]
        TCC0->CTRLA.bit.ENABLE = 1;                     // Enable the TCC0 counter
        #ifdef debug
            Serial.println("PWM Deadtime Changed at Stop");
        #endif
    }
  }
}


extern "C" void invGD_reset(uint8_T rst_status)
{
    switch(rst_status)
    {
        case 0:
            REG_PORT_OUTCLR0 = (1ul << pin_EN); break;   // Driver Disable 
            #ifdef debug
                Serial.println("GD Enable");
            #endif
        case 1:
            REG_PORT_OUTSET0 = (1ul << pin_EN); break;  // Driver Enable
            #ifdef debug
                Serial.println("GD Disable");
            #endif
        case 2:                                     // Driver Reset and Enable
            REG_PORT_OUTCLR0 = (1ul << pin_EN);
            delayMicroseconds(5);
            REG_PORT_OUTSET0 = (1ul << pin_EN);
            #ifdef debug
                Serial.println("GD Reset");
            #endif
            break;
        default: break;       
    }
}