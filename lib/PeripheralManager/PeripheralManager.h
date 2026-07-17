#pragma once

#include "ControllerConfig.h"
#include "Joystick.h"
#include "ButtonArray.h"

class c_PeripheralManager
{
public:
    c_PeripheralManager(c_Joystick *joy1, c_Joystick *joy2, c_ButtonArray *buttArr, uint8_t switchPin);
    void init();
    void updateVals();
    void getVals(PeripheralPacket &packet);

private:
    c_Joystick *_joy1;
    c_Joystick *_joy2;
    c_ButtonArray *_buttArr;
    uint8_t _switchPin;

    PeripheralPacket _peripheralPacket;
};