#pragma once

#include <Arduino.h>
#include "ControllerConfig.h"



class c_Joystick{
public:
    c_Joystick(uint8_t x_pin, uint8_t y_pin, uint8_t switch_pin, int deadband, float filter_alpha, float expo_factor ,int lowerLimit, int highigherLimit);
    void init();
    void update();
    JoystickData getVals();

private:
    int _meanSample(uint8_t sensPin);
    int _applyExpo(uint16_t val);

    uint8_t _x_pin;
    uint8_t _y_pin;
    uint8_t _switch_pin;

    int _baseline_x = 0;
    int _baseline_y = 0;

    int _lowerLimit = 0;
    int _higherLimit = 0;

    float _filtered_x = 0;
    float _filtered_y = 0;

    int _deadband = 0;
    float _filter_alpha = 0;
    float _expo_factor = 0;

};


