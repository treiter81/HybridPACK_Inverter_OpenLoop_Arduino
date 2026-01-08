#define uint32_T uint32_t
#define uint16_T uint16_t
#define uint8_T uint8_t


#ifndef _INVADC_ARDUINO_H_
#define _INVADC_ARDUINO_H_

#ifdef __cplusplus
extern "C" {
#endif

void invADC_init(void); //ADC init

void invADC_initports(void);  //init the ports

void invADC_usertrigger(void);    //can trigger the ADC from a user event

uint16_T invADC_read(uint8_T channel);   //read buffered ADC results

uint16_T invADC_readcounter(void);        //testfunction to see how many triggers occured

void ADC_Handler(void);            //read ADC result when ready and switches to the next sequencer channel

#ifdef __cplusplus
}
#endif

#endif //_INVADC_ARDUINO_H_