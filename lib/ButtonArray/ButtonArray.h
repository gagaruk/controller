#pragma once

#include <Arduino.h>

class c_ButtonArray
{
public:
    c_ButtonArray(uint8_t W_pin, uint8_t A_pin, uint8_t S_pin, uint8_t D_pin, uint8_t X_pin);
    void init();
    void update();

    bool isPressed(uint8_t buttonMask) const;
    bool isJustPressed(uint8_t buttonMask) const;

    ButtonData getPressedStruct() const;
    ButtonData getJustPressedStruct() const;

private:
    uint8_t _W_pin;
    uint8_t _A_pin;
    uint8_t _S_pin;
    uint8_t _D_pin;
    uint8_t _X_pin;

    uint8_t _currentStates = 0;
    uint8_t _previousStates = 0;

    unsigned long _lastDebounceTime = 0;
};