extern "C" void invPWM_dutycycle(uint16_T period, uint16_T Du, uint16_T Dv, uint16_T Dw) 
{
  // 1. Batch writes to CCB registers without waiting in between
  TCC0->CCB[0].reg = Du;
  TCC0->CCB[2].reg = Dv;
  TCC0->CCB[3].reg = Dw;

  // 2. Consolidate synchronization: Wait for all CCB bits at once
  // This is significantly faster than three separate while loops
  while (TCC0->SYNCBUSY.reg & (TCC_SYNCBUSY_CCB0 | TCC_SYNCBUSY_CCB2 | TCC_SYNCBUSY_CCB3));

  if (__builtin_expect(invPWMperiod != period, 0)) // Optimization: Hint that frequency rarely changes
  {
      invPWMperiod = period;

      // Update TCC0 Period
      TCC0->PERB.reg = period;
      while (TCC0->SYNCBUSY.bit.PERB);
      TCC0->CTRLBSET.reg = TCC_CTRLBSET_CMD_UPDATE; // Use standard macro for Update

      // Optimization: Pre-calculate shifted values
      uint32_t shiftVal = (period > 2000) ? 1 : 2;
      uint32_t newPer = (period << shiftVal) - 1;
      uint32_t newCC  = newPer - 99; // (period << shiftVal) - 100

      // Update TCC1
      TCC1->PERB.reg = newPer;
      TCC1->CCB[1].reg = newCC;
      while (TCC1->SYNCBUSY.reg & (TCC_SYNCBUSY_PERB | TCC_SYNCBUSY_CCB1));
      
      TCC1->CTRLBSET.reg = TCC_CTRLBSET_CMD_UPDATE;

      // Phase sync TCC0 and TCC1
      TCC0->CTRLBSET.reg = TCC_CTRLBSET_CMD_READSYNC;
      while (TCC0->SYNCBUSY.bit.COUNT);
      TCC1->COUNT.reg = TCC0->COUNT.reg;
      while (TCC1->SYNCBUSY.bit.COUNT); // Ensure TCC1 count is set before exiting

      #ifdef debug
        Serial.println(period);
      #endif                    
  }    
}