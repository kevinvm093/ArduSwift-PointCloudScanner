#ifndef KLCD_h
#define KLCD_h

#include <Wire.h>
#include "DeviceState.h"
#include <LiquidCrystal_I2C.h>


namespace KLCD 
{
    void Configure();
    void DisplayStatus(State& state);
}

#endif