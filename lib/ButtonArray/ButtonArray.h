#pragma once

#include <Arduino.h>

class c_buttonArray {
private:
    uint16_t _currentStates = 0;
    uint16_t _previousStates = 0;
    
    // Simple software debounce tracking
    unsigned long _lastDebounceTime = 0;
    const unsigned long DEBOUNCE_DELAY = 10; // 10ms

public:
    void init();
    void update();
    
    // Elegantly check button actions using the bitmasks
    bool isPressed(uint16_t buttonMask) const;     // True as long as it's held down
    bool isJustPressed(uint16_t buttonMask) const; // True ONLY on the exact frame it was clicked
};