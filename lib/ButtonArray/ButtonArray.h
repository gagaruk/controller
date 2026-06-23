#pragma once

#include <Arduino.h>

class c_ButtonArray {
public:
    c_ButtonArray();
    void init();
    void update();

    bool isPressed(uint16_t buttonMask) const;  
    bool isJustPressed(uint16_t buttonMask) const; 

    ButtonData getPressedStruct()const;
    ButtonData getJustPressedStruct() const;
private:
    uint16_t _currentStates = 0;
    uint16_t _previousStates = 0;
    
    unsigned long _lastDebounceTime = 0;
};