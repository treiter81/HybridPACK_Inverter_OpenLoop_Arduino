#include <Arduino.h>
#include "invIOfast.h"

extern "C" void Pin0TX_init(void)         //init the pin as output
{
   pinMode(0, OUTPUT);
   REG_PORT_DIRSET1 = PORT_PB22;
   REG_PORT_OUTCLR1 = PORT_PB22;
}

/*
extern "C" void Pin0TX_set(uint8_t pegel) //pin set
{
    //if (pegel == 0) {REG_PORT_OUTCLR1 = PORT_PB22;}
    //else {REG_PORT_OUTSET1 = PORT_PB22;}
    
    // pegel != 0 becomes 1, pegel == 0 becomes 0
    uint32_t offset = (pegel != 0);     
    // Calculate the address: OUTCLR address + (0 or 4 bytes)
    // Writing to OUTCLR + 4 is equivalent to writing to OUTSET
    *(&PORT_IOBUS->Group[1].OUTCLR.reg + offset) = PORT_PB22;
}
*/

extern "C" void Pin1RX_init(void)         //init the pin as output
{
   pinMode(1, OUTPUT);
   REG_PORT_DIRSET1 = PORT_PB23;
   REG_PORT_OUTCLR1 = PORT_PB23;
}


/*
extern "C" void Pin1RX_set(uint8_t pegel) //pin set
{
    //if (pegel == 0) {REG_PORT_OUTCLR1 = PORT_PB23;}
    //else {REG_PORT_OUTSET1 = PORT_PB23;}
    
    // pegel != 0 becomes 1, pegel == 0 becomes 0
    uint32_t offset = (pegel != 0);     
    // Calculate the address: OUTCLR address + (0 or 4 bytes)
    // Writing to OUTCLR + 4 is equivalent to writing to OUTSET
    *(&PORT_IOBUS->Group[1].OUTCLR.reg + offset) = PORT_PB23;
}
*/

//extern "C" void Pin1RX_toggle(void) //pin toggle
//{    
//    PORT_IOBUS->Group[1].OUTTGL.reg = PORT_PB23;
//}

