#pragma once

#include "ControllerConfig.h"
#include "Joystick.cpp"
#include "ButtonArray.cpp"

class c_PeripheralManager{
public:
    c_PeripheralManager(c_Joystick *joy1, c_Joystick *joy2, c_ButtonArray *buttArr, uint8_t potPin);
    void init();
    void updateVals();
    PeripheralPacket getVals();
private:
    c_Joystick* _joy1;
    c_Joystick* _joy2;
    c_ButtonArray* _buttArr;
    uint8_t _potPin;

    PeripheralPacket _peripheralPacket;
};