#ifndef _INVWD_ARDUINO_H_
#define _INVWD_ARDUINO_H_

#define uint8_T uint8_t

#ifdef __cplusplus
extern "C" {
#endif
void invWD_init(void); //Watchdog Init

void invWD_restart(uint8_T restart); //Watchdog Restart Now

#ifdef __cplusplus
}
#endif


#endif //_INVWD_ARDUINO_H_