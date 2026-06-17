#include "joystick.h"
#include "ControllerConstants.h"

c_Joystick::c_Joystick(uint8_t x_pin, uint8_t y_pin, uint8_t switch_pin, int deadband, float filter_alpha, float expo_factor,int lowerLimit, int higherLimit): 
    _x_pin(x_pin), _y_pin(y_pin), _switch_pin(switch_pin),
    _deadband(deadband), _filter_alpha(filter_alpha), _expo_factor(expo_factor),
    _lowerLimit(lowerLimit), _higherLimit(higherLimit){
}

void c_Joystick::init(){
    pinMode(_x_pin, INPUT);
    pinMode(_y_pin, INPUT);
    pinMode(_switch_pin, INPUT);

    _baseline_x = _meanSample(_x_pin);
    _baseline_y = _meanSample(_y_pin);
}

void c_Joystick::update(){
    int raw_x = constrain(analogRead(_x_pin), _lowerLimit, _higherLimit);
    int raw_y = constrain(analogRead(_y_pin), _lowerLimit, _higherLimit);

    _filtered_x = (_filter_alpha * raw_x) + ((1-_filter_alpha)* _filtered_x);
    _filtered_y = (_filter_alpha * raw_y) + ((1-_filter_alpha)* _filtered_y);
}


JoystickData c_Joystick::getVals(){
    JoystickData currentData;

    //Gets devation from the center
    int x_dev = int(_filtered_x) - _baseline_x;
    int y_dev = int(_filtered_y) - _baseline_y;

    //applies Dead Zone
    x_dev = abs(x_dev) < _deadband ? x_dev : 0;
    y_dev = abs(y_dev) < _deadband ? y_dev : 0;

    currentData.x_processed = _applyExpo(x_dev);
    currentData.y_processed = _applyExpo(y_dev);


    currentData.isPressed = digitalRead(_switch_pin);

}

int c_Joystick::_meanSample(uint8_t sensPin){
    int sum = 0;
    int start_t = millis();

    for(int sampleCount= 0; sampleCount<joystickConstants::operationalConstants::maxSampleCount; sampleCount++){

        sum+= constrain(analogRead(sensPin), _lowerLimit, _higherLimit);
        if(millis()-start_t > joystickConstants::operationalConstants::maxSampleDuration){
            return int(sum/sampleCount);
        }
    }

    return int(sum/joystickConstants::operationalConstants::maxSampleCount);

}

int c_Joystick::_applyExpo(uint16_t val){

    float val_norm = val/_higherLimit;
    
    float expo_norm = (1- _expo_factor)*val_norm + _expo_factor*(val_norm*val_norm*val_norm);

    return uint16_t(expo_norm * _higherLimit);
}