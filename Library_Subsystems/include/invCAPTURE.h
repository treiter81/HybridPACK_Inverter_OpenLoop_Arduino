#define uint32_T uint32_t
#define uint16_T uint16_t
#define uint8_T uint8_t

#ifndef _INVCAPTURE_ARDUINO_H_
#define _INVCAPTURE_ARDUINO_H_

#ifdef __cplusplus
extern "C" {
#endif

void invCapture_init(void);     //init the capture compare TC Modules

void TC4_Handler(void);             // Interrupt Service Routine (ISR) for timer TC4
void TC5_Handler(void);             // Interrupt Service Routine (ISR) for timer TC5

uint16_T invCapture_readData5(void); //read ADC data DATA5
uint16_T invCapture_readData6(void); //read ADC data DATA6


#ifdef __cplusplus
}
#endif

#endif //_INVCAPTURE_ARDUINO_H_