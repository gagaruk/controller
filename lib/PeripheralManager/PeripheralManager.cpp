#include "PeripheralManager.h"

c_PeripheralManager::c_PeripheralManager(c_Joystick &joy1, c_Joystick &joy2, c_ButtonArray &buttArr, uint8_t potPin):
    _joy1(joy1), _joy2(joy2), _buttArr(buttArr), _potPin(potPin) {}

void c_PeripheralManager::updateVals() {
    _joy1.update();
    _joy2.update();
    _buttArr.update();

    _peripheralPacket.joy1 = _joy1.getVals();
    _peripheralPacket.joy2 = _joy2.getVals();
    _peripheralPacket.buttArr = _buttArr.getPressedStruct();
    _peripheralPacket.potVal = analogRead(_potPin);
}
void c_PeripheralManager::updateVals() {
    _joy1.update();
    _joy2.update();
    _buttArr.update();

    _peripheralPacket.joy1 = _joy1.getVals();
    _peripheralPacket.joy2 = _joy2.getVals();
    _peripheralPacket.buttArr = _buttArr.getPressedStruct();
    _peripheralPacket.potVal = analogRead(_potPin);
}