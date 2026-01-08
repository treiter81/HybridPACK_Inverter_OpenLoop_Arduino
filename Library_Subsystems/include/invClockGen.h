#define uint32_T uint32_t
#define uint16_T uint16_t
#define uint8_T uint8_t

#ifndef _INVCLOCKGEN_ARDUINO_H_
#define _INVCLOCKGEN_ARDUINO_H_

#define GCLK_GENCTRL_SRC_DPLL96M_Val 0x8ul  // (GCLK_GENCTRL) DPLL96M output
#define GCLK_GENCTRL_SRC_DPLL96M (GCLK_GENCTRL_SRC_DPLL96M_Val << \
                                  GCLK_GENCTRL_SRC_Pos)
                                  

#ifdef __cplusplus
extern "C" {
#endif

void invClockGen_init(void); //init the generic clocks
uint8_T invClockGen_status(void); //status of generic clocks

#ifdef __cplusplus
}
#endif

#endif //_INVCLOCKGEN_ARDUINO_H_