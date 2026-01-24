/*
 * Code generation for system system '<S11>/ADC_RMS'
 *
 * Model                      : INVERTER_Arduino
 * Model version              : 217
 * Simulink Coder version : 9.8 (R2022b) 13-May-2022
 * C source code generated on : Sat Jan 24 22:54:10 2026
 *
 * Note that the functions contained in this file are part of a Simulink
 * model, and are not self-contained algorithms.
 */

#include "ADC_RMS.h"
#include "rtwtypes.h"
#include "INVERTER_Arduino_private.h"
#include "INVERTER_Arduino.h"

/* Named constants for Chart: '<S11>/ADC_RMS' */
#define INVERTER_Ar_IN_Sqrt_final_value ((uint8_T)7U)
#define INVERTER_Ard_IN_NO_ACTIVE_CHILD ((uint8_T)0U)
#define INVERTER_Ardu_IN_ADC_WaitSteady ((uint8_T)2U)
#define INVERTER_Arduin_IN_ADC_GetReady ((uint8_T)1U)
#define INVERTER_Arduino_IN_DIV_i      ((uint8_T)3U)
#define INVERTER_Arduino_IN_GETOFFSET  ((uint8_T)4U)
#define INVERTER_Arduino_IN_OFFSET     ((uint8_T)5U)
#define INVERTER_Arduino_IN_SUM_UP     ((uint8_T)6U)
#define INVERTE_IN_Sqrt_iterative_step1 ((uint8_T)8U)
#define INVERTE_IN_Sqrt_iterative_step2 ((uint8_T)9U)

/* System initialize for atomic system: '<S11>/ADC_RMS' */
void INVERTER_Ardui_ADC_RMS_Init(B_ADC_RMS_INVERTER_Arduino_T *localB,
  DW_ADC_RMS_INVERTER_Arduino_T *localDW)
{
  localDW->temporalCounter_i1 = 0.0;
  localDW->is_active_c6_INVERTER_Arduino = 0U;
  localDW->is_c6_INVERTER_Arduino = INVERTER_Ard_IN_NO_ACTIVE_CHILD;
  localDW->ADCUbuf = 0;
  localDW->ADCVbuf = 0;
  localDW->ADCWbuf = 0;
  localDW->OffsetV = 0;
  localDW->OffsetW = 0;
  localDW->i = 0U;
  localDW->OffsetU = 0;
  localDW->RMSsumU = 0U;
  localDW->RMSsumW = 0U;
  localDW->RMSsumV = 0U;
  localDW->tmpU = 0U;
  localDW->SU = 0U;
  localDW->SW = 0U;
  localDW->SV = 0U;
  localDW->tmpV = 0U;
  localDW->tmpW = 0U;
  localB->ADCUout = 0;
  localB->ADCVout = 0;
  localB->ADCWout = 0;
  localDW->presentTime = 0.0;
  localDW->elapsedTime = 0.0;
  localDW->previousTime = 0.0;
}

/* System reset for atomic system: '<S11>/ADC_RMS' */
void INVERTER_Ardu_ADC_RMS_Reset(B_ADC_RMS_INVERTER_Arduino_T *localB,
  DW_ADC_RMS_INVERTER_Arduino_T *localDW)
{
  localDW->temporalCounter_i1 = 0.0;
  localDW->is_active_c6_INVERTER_Arduino = 0U;
  localDW->is_c6_INVERTER_Arduino = INVERTER_Ard_IN_NO_ACTIVE_CHILD;
  localDW->ADCUbuf = 0;
  localDW->ADCVbuf = 0;
  localDW->ADCWbuf = 0;
  localDW->OffsetV = 0;
  localDW->OffsetW = 0;
  localDW->i = 0U;
  localDW->OffsetU = 0;
  localDW->RMSsumU = 0U;
  localDW->RMSsumW = 0U;
  localDW->RMSsumV = 0U;
  localDW->tmpU = 0U;
  localDW->SU = 0U;
  localDW->SW = 0U;
  localDW->SV = 0U;
  localDW->tmpV = 0U;
  localDW->tmpW = 0U;
  localB->ADCUout = 0;
  localB->ADCVout = 0;
  localB->ADCWout = 0;
  localDW->presentTime = 0.0;
  localDW->elapsedTime = 0.0;
  localDW->previousTime = 0.0;
}

/* Enable for atomic system: '<S11>/ADC_RMS' */
void INVERTER_Ard_ADC_RMS_Enable(RT_MODEL_INVERTER_Arduino_T * const
  INVERTER_Arduino_M, DW_ADC_RMS_INVERTER_Arduino_T *localDW)
{
  localDW->presentTime = ((INVERTER_Arduino_M->Timing.clockTick2) * 0.01);
  localDW->previousTime = localDW->presentTime;
}

/* Disable for atomic system: '<S11>/ADC_RMS' */
void INVERTER_Ar_ADC_RMS_Disable(RT_MODEL_INVERTER_Arduino_T * const
  INVERTER_Arduino_M, DW_ADC_RMS_INVERTER_Arduino_T *localDW)
{
  localDW->presentTime = ((INVERTER_Arduino_M->Timing.clockTick2) * 0.01);
  localDW->elapsedTime = localDW->presentTime - localDW->previousTime;
  localDW->previousTime = localDW->presentTime;
  localDW->temporalCounter_i1 += localDW->elapsedTime;
}

/* Output and update for atomic system: '<S11>/ADC_RMS' */
void INVERTER_Arduino_ADC_RMS(RT_MODEL_INVERTER_Arduino_T * const
  INVERTER_Arduino_M, uint16_T rtu_polarity, uint16_T rtu_ADCU, uint16_T
  rtu_ADCV, uint16_T rtu_ADCW, B_ADC_RMS_INVERTER_Arduino_T *localB,
  DW_ADC_RMS_INVERTER_Arduino_T *localDW)
{
  localDW->presentTime = ((INVERTER_Arduino_M->Timing.clockTick2) * 0.01);
  localDW->elapsedTime = localDW->presentTime - localDW->previousTime;
  localDW->previousTime = localDW->presentTime;
  localDW->temporalCounter_i1 += localDW->elapsedTime;
  localDW->polarity_prev = localDW->polarity_start;
  localDW->polarity_start = rtu_polarity;

  /* Chart: '<S11>/ADC_RMS' */
  if (localDW->is_active_c6_INVERTER_Arduino == 0U) {
    localDW->polarity_prev = rtu_polarity;
    localDW->is_active_c6_INVERTER_Arduino = 1U;
    localDW->is_c6_INVERTER_Arduino = INVERTER_Arduin_IN_ADC_GetReady;
  } else {
    switch (localDW->is_c6_INVERTER_Arduino) {
     case INVERTER_Arduin_IN_ADC_GetReady:
      if (((rtu_ADCU > 0) & (rtu_ADCV > 0) & (rtu_ADCW > 0)) != 0) {
        localDW->is_c6_INVERTER_Arduino = INVERTER_Ardu_IN_ADC_WaitSteady;
        localDW->temporalCounter_i1 = 0.0;
      }
      break;

     case INVERTER_Ardu_IN_ADC_WaitSteady:
      if (((((INVERTER_Arduino_M->Timing.clockTick2) * 0.01) -
            localDW->previousTime) + localDW->temporalCounter_i1) * 1000.0 >=
          200.0) {
        localDW->is_c6_INVERTER_Arduino = INVERTER_Arduino_IN_GETOFFSET;
        localDW->i = 1U;
        localDW->ADCUbuf += (int16_T)rtu_ADCU;
        localDW->ADCVbuf += (int16_T)rtu_ADCV;
        localDW->ADCWbuf += (int16_T)rtu_ADCW;
      }
      break;

     case INVERTER_Arduino_IN_DIV_i:
      localDW->i = 1U;
      localDW->SU = 1000U;
      localDW->SV = 1000U;
      localDW->SW = 1000U;
      localDW->is_c6_INVERTER_Arduino = INVERTE_IN_Sqrt_iterative_step1;

      /* iterative sqrt with NewtonRaphson Algo */
      localDW->tmpU = localDW->RMSsumU / 1000U;
      localDW->tmpV = localDW->RMSsumV / 1000U;
      localDW->tmpW = localDW->RMSsumW / 1000U;
      break;

     case INVERTER_Arduino_IN_GETOFFSET:
      if (localDW->i == 4) {
        localDW->is_c6_INVERTER_Arduino = INVERTER_Arduino_IN_OFFSET;
        localDW->OffsetU = (int16_T)(localDW->ADCUbuf >> 2);
        localDW->OffsetV = (int16_T)(localDW->ADCVbuf >> 2);
        localDW->OffsetW = (int16_T)(localDW->ADCWbuf >> 2);
      } else {
        localDW->i++;
        localDW->ADCUbuf += (int16_T)rtu_ADCU;
        localDW->ADCVbuf += (int16_T)rtu_ADCV;
        localDW->ADCWbuf += (int16_T)rtu_ADCW;
      }
      break;

     case INVERTER_Arduino_IN_OFFSET:
      {
        if ((localDW->polarity_prev != localDW->polarity_start) &&
            (localDW->polarity_start == 1)) {
          int32_T RMSsumU_tmp;
          localDW->i = 1U;
          localDW->is_c6_INVERTER_Arduino = INVERTER_Arduino_IN_SUM_UP;
          RMSsumU_tmp = rtu_ADCU - localDW->OffsetU;
          localDW->RMSsumU = (uint32_T)(RMSsumU_tmp * RMSsumU_tmp);
          RMSsumU_tmp = rtu_ADCV - localDW->OffsetV;
          localDW->RMSsumV = (uint32_T)(RMSsumU_tmp * RMSsumU_tmp);
          RMSsumU_tmp = rtu_ADCW - localDW->OffsetW;
          localDW->RMSsumW = (uint32_T)(RMSsumU_tmp * RMSsumU_tmp);
        } else {
          localDW->OffsetU = (int16_T)(localDW->ADCUbuf >> 2);
          localDW->OffsetV = (int16_T)(localDW->ADCVbuf >> 2);
          localDW->OffsetW = (int16_T)(localDW->ADCWbuf >> 2);
        }
      }
      break;

     case INVERTER_Arduino_IN_SUM_UP:
      {
        if ((((localDW->polarity_prev != localDW->polarity_start) &&
              (localDW->polarity_start == 1)) | (localDW->i > 1000)) != 0) {
          /* assumption is ratiometric output (max val 0.5*4096). Overflow will not occur before 1024 */
          localDW->is_c6_INVERTER_Arduino = INVERTER_Arduino_IN_DIV_i;
          if (localDW->i == 0U) {
            localDW->RMSsumU = MAX_uint32_T;

            /* Divide by zero handler */
          } else {
            localDW->RMSsumU /= localDW->i;
          }

          if (localDW->i == 0U) {
            localDW->RMSsumV = MAX_uint32_T;

            /* Divide by zero handler */
          } else {
            localDW->RMSsumV /= localDW->i;
          }

          if (localDW->i == 0U) {
            localDW->RMSsumW = MAX_uint32_T;

            /* Divide by zero handler */
          } else {
            localDW->RMSsumW /= localDW->i;
          }
        } else {
          int32_T RMSsumU_tmp;
          localDW->i++;
          localDW->is_c6_INVERTER_Arduino = INVERTER_Arduino_IN_SUM_UP;
          RMSsumU_tmp = rtu_ADCU - localDW->OffsetU;
          localDW->RMSsumU += (uint32_T)(RMSsumU_tmp * RMSsumU_tmp);
          RMSsumU_tmp = rtu_ADCV - localDW->OffsetV;
          localDW->RMSsumV += (uint32_T)(RMSsumU_tmp * RMSsumU_tmp);
          RMSsumU_tmp = rtu_ADCW - localDW->OffsetW;
          localDW->RMSsumW += (uint32_T)(RMSsumU_tmp * RMSsumU_tmp);
        }
      }
      break;

     case INVERTER_Ar_IN_Sqrt_final_value:
      if ((localDW->polarity_prev != localDW->polarity_start) &&
          (localDW->polarity_start == 1)) {
        localDW->i = 1U;
        localDW->is_c6_INVERTER_Arduino = INVERTER_Arduino_IN_SUM_UP;
        localDW->RMSsumU = (uint32_T)((rtu_ADCU - localDW->OffsetU) * (rtu_ADCU
          - localDW->OffsetU));
        localDW->RMSsumV = (uint32_T)((rtu_ADCV - localDW->OffsetV) * (rtu_ADCV
          - localDW->OffsetV));
        localDW->RMSsumW = (uint32_T)((rtu_ADCW - localDW->OffsetW) * (rtu_ADCW
          - localDW->OffsetW));
      }
      break;

     case INVERTE_IN_Sqrt_iterative_step1:
      localDW->is_c6_INVERTER_Arduino = INVERTE_IN_Sqrt_iterative_step2;
      localDW->SU = (localDW->SU + localDW->tmpU) >> 1;
      localDW->SV = (localDW->SV + localDW->tmpV) >> 1;
      localDW->SW = (localDW->SW + localDW->tmpW) >> 1;
      break;

     default:
      /* case IN_Sqrt_iterative_step2: */
      if (localDW->i > 8) {
        /* typ 7 iterations are enough */
        localDW->is_c6_INVERTER_Arduino = INVERTER_Ar_IN_Sqrt_final_value;
        localB->ADCUout = (int16_T)localDW->SU;
        localB->ADCVout = (int16_T)localDW->SV;
        localB->ADCWout = (int16_T)localDW->SW;
      } else {
        localDW->i++;
        localDW->is_c6_INVERTER_Arduino = INVERTE_IN_Sqrt_iterative_step1;

        /* iterative sqrt with NewtonRaphson Algo */
        if (localDW->SU == 0U) {
          localDW->tmpU = MAX_uint32_T;

          /* Divide by zero handler */
        } else {
          localDW->tmpU = localDW->RMSsumU / localDW->SU;
        }

        if (localDW->SV == 0U) {
          localDW->tmpV = MAX_uint32_T;

          /* Divide by zero handler */
        } else {
          localDW->tmpV = localDW->RMSsumV / localDW->SV;
        }

        if (localDW->SW == 0U) {
          localDW->tmpW = MAX_uint32_T;

          /* Divide by zero handler */
        } else {
          localDW->tmpW = localDW->RMSsumW / localDW->SW;
        }
      }
      break;
    }
  }

  /* End of Chart: '<S11>/ADC_RMS' */
}
