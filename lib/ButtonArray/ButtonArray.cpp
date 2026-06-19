#include "ControllerConfig.h"
#include "ButtonArray.h"

void c_buttonArray::init() {
    pinMode(ButtonArray::pins::BTN_X, INPUT_PULLUP);
    pinMode(ButtonArray::pins::BTN_CIRCLE, INPUT_PULLUP);
    pinMode(ButtonArray::pins::BTN_TRIANGLE, INPUT_PULLUP);
    pinMode(ButtonArray::pins::BTN_SQUARE, INPUT_PULLUP);
    pinMode(ButtonArray::pins::BTN_L1, INPUT_PULLUP);
    pinMode(ButtonArray::pins::BTN_R1, INPUT_PULLUP);
}

void c_buttonArray::update() {
    if (millis() - _lastDebounceTime < DEBOUNCE_DELAY) return;
    _lastDebounceTime = millis();

    _previousStates = _currentStates;
    uint16_t freshStates = 0;

    if (digitalRead(ButtonArray::pins::BTN_X) == LOW)        freshStates |= ButtonArray::masks::MASK_X;
    if (digitalRead(ButtonArray::pins::BTN_CIRCLE) == LOW)   freshStates |= ButtonArray::masks::MASK_CIRCLE;
    if (digitalRead(ButtonArray::pins::BTN_TRIANGLE) == LOW) freshStates |= ButtonArray::masks::MASK_TRIANGLE;
    if (digitalRead(ButtonArray::pins::BTN_SQUARE) == LOW)   freshStates |= ButtonArray::masks::MASK_SQUARE;
    if (digitalRead(ButtonArray::pins::BTN_L1) == LOW)       freshStates |= ButtonArray::masks::MASK_L1;
    if (digitalRead(ButtonArray::pins::BTN_R1) == LOW)       freshStates |= ButtonArray::masks::MASK_R1;

    _currentStates = freshStates;
}

bool c_buttonArray::isPressed(uint16_t buttonMask) const {
    return (_currentStates & buttonMask) != 0;
}

bool c_buttonArray::isJustPressed(uint16_t buttonMask) const {

    return ((_currentStates & buttonMask) != 0) && ((_previousStates & buttonMask) == 0);
}

ButtonData c_buttonArray::getPressedStruct()const{
    ButtonData data;
    data.x = isPressed(ButtonArray::masks::MASK_X);
    data.circle = isPressed(ButtonArray::masks::MASK_CIRCLE);
    data.triangle = (ButtonArray::masks::MASK_TRIANGLE);
    data.square = isPressed(ButtonArray::masks::MASK_SQUARE);
    data.l1 = isPressed(ButtonArray::masks::MASK_L1);
    data.r1 = isPressed(ButtonArray::masks::MASK_R1);

    return data;

}

ButtonData c_buttonArray::getJustPressedStruct() const{
    ButtonData data;
    data.x = isJustPressed(ButtonArray::masks::MASK_X);
    data.circle = isJustPressed(ButtonArray::masks::MASK_CIRCLE);
    data.triangle = (ButtonArray::masks::MASK_TRIANGLE);
    data.square = isJustPressed(ButtonArray::masks::MASK_SQUARE);
    data.l1 = isJustPressed(ButtonArray::masks::MASK_L1);
    data.r1 = isJustPressed(ButtonArray::masks::MASK_R1);

    return data;

}