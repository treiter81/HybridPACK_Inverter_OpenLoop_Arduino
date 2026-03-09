#include <Arduino.h>
#include "getLock_info.h"

extern "C" void getLock_info(uint16_T input) //Info about Communication Locked Mode
{
    if (input==1000)
    {
        Serial.println("Mode Unlocked");
    }
    else if (input==1001)
    {
        Serial.println("Lock Command 1");
    }
    else if (input==1002)
    {
        Serial.println("Lock Command 2");
    }
    else if (input==1003)
    {
        Serial.println("Lock Command 3");
    }
    else if (input==0)
    {
        Serial.println("Message Ignored by Filter");
    }
    else
    {
        Serial.print("Locked to ID: ");
        Serial.println(input);
    }
}


