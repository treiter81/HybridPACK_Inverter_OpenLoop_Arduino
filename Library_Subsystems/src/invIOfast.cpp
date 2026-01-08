#include <Arduino.h>
#include "invIOfast.h"

extern "C" void Pin0TX_init(void)         //init the pin as output
{
   pinMode(0, OUTPUT);
   REG_PORT_DIRSET1 = PORT_PB22;
   REG_PORT_OUTCLR1 = PORT_PB22;
}

extern "C" void Pin0TX_set(uint8_t pegel) //pin set
{
    if (pegel == 0) {REG_PORT_OUTCLR1 = PORT_PB22;}
    else {REG_PORT_OUTSET1 = PORT_PB22;}
}

extern "C" void Pin1RX_init(void)         //init the pin as output
{
   pinMode(1, OUTPUT);
   REG_PORT_DIRSET1 = PORT_PB23;
   REG_PORT_OUTCLR1 = PORT_PB23;
}

extern "C" void Pin1RX_set(uint8_t pegel) //pin set
{
    if (pegel == 0) {REG_PORT_OUTCLR1 = PORT_PB23;}
    else {REG_PORT_OUTSET1 = PORT_PB23;}
}
