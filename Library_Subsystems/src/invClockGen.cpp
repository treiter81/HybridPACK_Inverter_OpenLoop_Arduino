#include <Arduino.h>
#include "invClockGen.h"

extern "C" uint8_T invClockGeninitdone = 0;

/* Generic Clock init as
ID2: 1024 Hz  //used for WatchDog
ID3: 8 MHz    //used for ADCs
ID4: 48 MHz   //used for generic and 100MHz PLL
ID5: 100 MHz  //used for PWM and Compare Capture
*/

extern "C" void invClockGen_init(void) //init the generic clocks
{
  
  if (invClockGeninitdone == 0)
  {
  
  GCLK->CLKCTRL.reg = GCLK_CLKCTRL_CLKEN |        // Enable generic clock
                      GCLK_CLKCTRL_GEN_GCLK2 |    // Select GCLK2
                      GCLK_CLKCTRL_ID_WDT;        // Route Watchdogtimer
  while (GCLK->STATUS.bit.SYNCBUSY);              // Wait for synchronization
  
  GCLK->GENDIV.reg = GCLK_GENDIV_ID(2) | GCLK_GENDIV_DIV(4);
  // Enable clock generator 2 using low-power 32KHz oscillator.
  // With /32 will be 1024 Hz Clock
  GCLK->GENCTRL.reg = GCLK_GENCTRL_ID(2) |
                      GCLK_GENCTRL_GENEN |
                      GCLK_GENCTRL_SRC_OSCULP32K |
                      GCLK_GENCTRL_DIVSEL;
  while (GCLK->STATUS.bit.SYNCBUSY);


  // Configure generic clock generator 3
  GCLK->GENCTRL.reg = GCLK_GENCTRL_IDC |          // Improve duty cycle
                      GCLK_GENCTRL_GENEN |        // Enable generic clock gen
                      GCLK_GENCTRL_SRC_OSC8M |    // Select 8MHz as source
                      GCLK_GENCTRL_ID(3);         // Apply to GCLK4
  while (GCLK->STATUS.bit.SYNCBUSY);              // Wait for synchronization



  // Configure generic clock generator 4
  GCLK->GENCTRL.reg = GCLK_GENCTRL_IDC |          // Improve duty cycle
                      GCLK_GENCTRL_GENEN |        // Enable generic clock gen
                      GCLK_GENCTRL_SRC_DFLL48M |  // Select 48MHz as source
                      GCLK_GENCTRL_ID(4);         // Apply to GCLK4
  while (GCLK->STATUS.bit.SYNCBUSY);              // Wait for synchronization

  // Set clock divider of 48 to generic clock generator 4 (1 MHz)
  GCLK->GENDIV.reg = GCLK_GENDIV_DIV(48) |        // Divide 48 MHz by 48
                     GCLK_GENDIV_ID(4);           // Apply to GCLK4 4
  while (GCLK->STATUS.bit.SYNCBUSY);              // Wait for synchronization

  // Enable GCLK4 and connect it to GCLK_DPLL
  GCLK->CLKCTRL.reg = GCLK_CLKCTRL_CLKEN |        // Enable generic clock
                      GCLK_CLKCTRL_GEN_GCLK4 |    // Select GCLK4
                      GCLK_CLKCTRL_ID(1);         // Feed GCLK4 to GCLK_DPLL
  while (GCLK->STATUS.bit.SYNCBUSY);              // Wait for synchronization

  // Set DPLL ratio to 1 MHz * (99 + 1) = 100 MHz
  SYSCTRL->DPLLRATIO.reg = SYSCTRL_DPLLRATIO_LDRFRAC(0) | // Ratio fractional
                           SYSCTRL_DPLLRATIO_LDR(99);     // Ratio

  // Configure DPLL to disregard phase lock and select GCLK as source
  SYSCTRL->DPLLCTRLB.reg = SYSCTRL_DPLLCTRLB_LBYPASS |    // Bypass lock
                           SYSCTRL_DPLLCTRLB_WUF |        // Wake up fast
    SYSCTRL_DPLLCTRLB_REFCLK(SYSCTRL_DPLLCTRLB_REFCLK_GCLK_Val); // Select GCLK
  
  // Enable DPLL
  SYSCTRL->DPLLCTRLA.reg |= SYSCTRL_DPLLCTRLA_ENABLE;
  
  // Configure generic clock generator 5
  GCLK->GENCTRL.reg = GCLK_GENCTRL_IDC |          // Improve duty cycle
                      GCLK_GENCTRL_GENEN |        // Enable generic clock gen
                      GCLK_GENCTRL_SRC_DPLL96M |  // Select DPLL as source
                      GCLK_GENCTRL_ID(5);         // Apply to GCLK5
  while (GCLK->STATUS.bit.SYNCBUSY);              // Wait for synchronization

  // Set clock divider of 1 to generic clock generator 5 (100 MHz)
  GCLK->GENDIV.reg = GCLK_GENDIV_DIV(1) |        // Divide 100 MHz by 1
                     GCLK_GENDIV_ID(5);           // Apply to GCLK5
  while (GCLK->STATUS.bit.SYNCBUSY);              // Wait for synchronization

  invClockGeninitdone = 1;
  Serial.println("Generic Clock Setup done");
  }
  
  else{Serial.println("Generic Clock already running");}
}

extern "C" uint8_T invClockGen_status(void) //return the status of generic clock init
{
  return (invClockGeninitdone);
}

