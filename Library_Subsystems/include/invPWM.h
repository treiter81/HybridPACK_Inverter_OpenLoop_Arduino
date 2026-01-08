#ifndef _INVPWM_ARDUINO_H_
#define _INVPWM_ARDUINO_H_

#define pin_LSU 6 //sys2
#define pin_HSU 2 //sys1
#define pin_LSV 8 //sys4
#define pin_HSV 9 //sys3
#define pin_LSW 16 //sys6
#define pin_HSW 10 //sys5

#define pin_EN 5  //enable

#define pin_CS 4  //chip select and IRQ trigger for the control loops


#define uint32_T uint32_t
#define uint16_T uint16_t
#define uint8_T uint8_t


#ifdef __cplusplus
extern "C" {
#endif
void invPWM_init(void); //PWM init with TCC0 as counter

void invPWM_enable(uint8_T enable);                 //enable or disable the PWM (without double buffer update)

void invPWM_start(void);                            //update registers and start the PWM

void invPWM_stop(void);                             //stop the PWM

void invPWM_dutycycle(uint16_T period, uint16_T Du, uint16_T Dv, uint16_T Dw); //Duty Cycles, Period
uint8_T invPWM_status(void);                        //output the PWM enable status

void invPWM_deadtime(uint8_T deadtime);             //check PWM status and apply new deadtime values

void invGD_reset(uint8_T rst_status);

#ifdef __cplusplus
}
#endif


#endif //_INVPWM_ARDUINO_H_