#include <Arduino.h>
#include "invWD.h"
#include "invClockGen.h"

extern "C" void invWD_init(void) //Watchdog init 
{
  
  if (invClockGen_status() == 0)    //if generic clocks are not running; start them
  {
    invClockGen_init();         
  }

  // WDT clock = clock gen 2
  GCLK->CLKCTRL.reg = GCLK_CLKCTRL_ID_WDT |
                      GCLK_CLKCTRL_CLKEN |
                      GCLK_CLKCTRL_GEN_GCLK2;
 }


extern "C" void invWD_restart(uint8_T restart)
{
  if (restart  == 1)
  {
    WDT->CONFIG.bit.PER = 0x1;             
    while(WDT->STATUS.bit.SYNCBUSY);                  // Wait for synchronization

    REG_WDT_CTRL = WDT_CTRL_ENABLE;                   // Enable the WDT in normal mode
    while(WDT->STATUS.bit.SYNCBUSY);                  // Wait for synchronization
  }      
}
