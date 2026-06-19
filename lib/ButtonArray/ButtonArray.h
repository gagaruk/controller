#pragma once

#include <Arduino.h>

class c_buttonArray {
private:
    uint16_t _currentStates = 0;
    uint16_t _previousStates = 0;
    
    unsigned long _lastDebounceTime = 0;
    const unsigned long DEBOUNCE_DELAY = 10; // 10ms

public:
    void init();
    void update();

    bool isPressed(uint16_t buttonMask) const;  
    bool isJustPressed(uint16_t buttonMask) const; 

    ButtonData getPressedStruct()const;
    ButtonData getJustPressedStruct() const;
};