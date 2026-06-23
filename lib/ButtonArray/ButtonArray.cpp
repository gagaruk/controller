#include "ControllerConfig.h"
#include "ButtonArray.h"

void c_ButtonArray::init() {
    pinMode(ButtonArray::pins::L1, INPUT_PULLUP);
    pinMode(ButtonArray::pins::L2, INPUT_PULLUP);
    pinMode(ButtonArray::pins::R1, INPUT_PULLUP);
    pinMode(ButtonArray::pins::R2, INPUT_PULLUP);
 
}

void c_ButtonArray::update() {
    if (millis() - _lastDebounceTime < DEBOUNCE_DELAY) return;
    _lastDebounceTime = millis();

    _previousStates = _currentStates;
    uint16_t freshStates = 0;

    if (digitalRead(ButtonArray::pins::L1) == LOW)        freshStates |= ButtonArray::masks::L1;
    if (digitalRead(ButtonArray::pins::L2) == LOW)   freshStates |= ButtonArray::masks::L2;
    if (digitalRead(ButtonArray::pins::R1) == LOW) freshStates |= ButtonArray::masks::R1;
    if (digitalRead(ButtonArray::pins::R2) == LOW)   freshStates |= ButtonArray::masks::R2;

    _currentStates = freshStates;
}

bool c_ButtonArray::isPressed(uint16_t buttonMask) const {
    return (_currentStates & buttonMask) != 0;
}

bool c_ButtonArray::isJustPressed(uint16_t buttonMask) const {

    return ((_currentStates & buttonMask) != 0) && ((_previousStates & buttonMask) == 0);
}

ButtonData c_ButtonArray::getPressedStruct()const{
    ButtonData data;
    data.l1 = isPressed(ButtonArray::masks::L1);
    data.l2 = isPressed(ButtonArray::masks::L2);
    data.r1 = isPressed(ButtonArray::masks::R1);
    data.r2 = isPressed(ButtonArray::masks::R2);

    return data;

}

ButtonData c_ButtonArray::getJustPressedStruct() const{
    ButtonData data;
    data.l1 = isJustPressed(ButtonArray::masks::L1);
    data.l2 = isJustPressed(ButtonArray::masks::L2);
    data.r1 = isJustPressed(ButtonArray::masks::R1);
    data.r2 = isJustPressed(ButtonArray::masks::R2);

    return data;

}