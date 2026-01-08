#define uint32_T uint32_t
#define uint16_T uint16_t
#define uint8_T uint8_t

#ifndef _INVSPI_ARDUINO_H_
#define _INVSPI_ARDUINO_H_

//Chip Select from PWM library
//#define pin_CS 4

#define pin_SCK 13 //SPI PIN SCK
#define pin_MISO 12 //SPI PIN 
#define pin_MOSI 11 //SPI PIN 

#define ADS_bitshift 0
#define SPI_MHz 2           //SCK Speed
#define SPI_CLOCK_MHz 48    //Peripheral clock speeed
#define SPI_bytes 2         //bytes to transfer


#ifdef __cplusplus
extern "C" {
#endif

void invSPI_init(void);  //SPI init

uint16_t readwriteSPI(uint8_t data,uint8_t byte_pos);  //SPI Read 16bit data and return result from last SPI Transfers

void SERCOM1_Handler(void); //the interrupt routine to transfer 2 bytes

#ifdef __cplusplus
}
#endif

#endif //_INVSPI_ARDUINO_H_