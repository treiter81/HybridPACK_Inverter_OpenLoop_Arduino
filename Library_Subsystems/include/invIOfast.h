#ifndef _INVIOFAST_ARDUINO_H_
#define _INVIOFAST_ARDUINO_H_

#define uint32_T uint32_t
#define uint16_T uint16_t
#define uint8_T uint8_t

#ifdef __cplusplus
extern "C" {
#endif

void Pin0TX_init(void);         //init the pin as output
void Pin0TX_set(uint8_t pegel); //pin set

void Pin1RX_init(void);         //init the pin as output
void Pin1RX_set(uint8_t pegel); //pin set


#ifdef __cplusplus
}
#endif


#endif //_INVIOFAST_ARDUINO_H_