#pragma once

#include "ControllerConfig.h"
#include "Joystick.cpp"
#include "ButtonArray.cpp"

class c_peripheralManager{
public:
    c_peripheralManager(c_Joystick joy1, c_Joystick joy2, c_buttonArray buttArr, uint8_t potPin);
    void updateVals();
    Transf getVals();


private:

};