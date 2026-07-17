#include "PeripheralManager.h"

c_PeripheralManager::c_PeripheralManager(c_Joystick *joy1, c_Joystick *joy2, c_ButtonArray *buttArr, uint8_t switchPin) : _joy1(joy1), _joy2(joy2), _buttArr(buttArr), _switchPin(switchPin) {}

void c_PeripheralManager::init()
{
    _joy1->init();
    _joy2->init();
    _buttArr->init();
    pinMode(_switchPin, INPUT);
}

void c_PeripheralManager::updateVals()
{
    _joy1->update();
    _joy2->update();
    _buttArr->update();

    _peripheralPacket.joy1 = _joy1->getVals();
    LOG_JOYSTICK(_peripheralPacket.joy1.x_processed, _peripheralPacket.joy1.y_processed);
    _peripheralPacket.joy2 = _joy2->getVals();
    LOG_JOYSTICK(_peripheralPacket.joy2.x_processed, _peripheralPacket.joy2.y_processed);
    _peripheralPacket.buttArr = _buttArr->getPressedStruct();
    LOG_BUTTONS(_peripheralPacket.buttArr.w, _peripheralPacket.buttArr.a, _peripheralPacket.buttArr.s, _peripheralPacket.buttArr.d, _peripheralPacket.buttArr.x);
    _peripheralPacket.switchState = digitalRead(_switchPin);
    LOG_SWITCH(_peripheralPacket.switchState);
}

void c_PeripheralManager::getVals(PeripheralPacket &packet)
{
    packet = _peripheralPacket;
}