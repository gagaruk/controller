#include "joystick.h"
#include "ControllerConfig.h"

c_Joystick::c_Joystick(uint8_t x_pin, uint8_t y_pin, uint8_t switch_pin, int deadband, float filter_alpha, float expo_factor, int lowerLimit, int higherLimit)
    : _x_pin(x_pin), _y_pin(y_pin), _switch_pin(switch_pin),
      _deadband(deadband), _filter_alpha(filter_alpha), _expo_factor(expo_factor),
      _lowerLimit(lowerLimit), _higherLimit(higherLimit)
{
}

void c_Joystick::init()
{
    pinMode(_x_pin, INPUT);
    pinMode(_y_pin, INPUT);
    pinMode(_switch_pin, INPUT_PULLUP);

    delay(150);

    _baseline_x = _meanSample(_x_pin);
    _baseline_y = _meanSample(_y_pin);

    _filtered_x = _baseline_x;
    _filtered_y = _baseline_y;
}

void c_Joystick::update()
{
    int raw_x = constrain(analogRead(_x_pin), 0, 4095);
    int raw_y = constrain(analogRead(_y_pin), 0, 4095);

    _filtered_x = (_filter_alpha * raw_x) + ((1 - _filter_alpha) * _filtered_x);
    _filtered_y = (_filter_alpha * raw_y) + ((1 - _filter_alpha) * _filtered_y);
}

JoystickData c_Joystick::getVals()
{
    JoystickData currentData;

    int x_dev = int(_filtered_x) - _baseline_x;
    int y_dev = int(_filtered_y) - _baseline_y;

    int x_sign = (x_dev >= 0) ? 1 : -1;
    int y_sign = (y_dev >= 0) ? 1 : -1;

    x_dev = abs(x_dev) > _deadband ? abs(x_dev) : 0;
    y_dev = abs(y_dev) > _deadband ? abs(y_dev) : 0;

    int raw_max_deviation = 2048;
    uint16_t expo_x = _applyExpo(x_dev, raw_max_deviation);
    uint16_t expo_y = _applyExpo(y_dev, raw_max_deviation);

    currentData.x = map(expo_x, 0, raw_max_deviation, 0, _higherLimit) * x_sign;
    currentData.y = map(expo_y, 0, raw_max_deviation, 0, _higherLimit) * y_sign;

    currentData.sw = !digitalRead(_switch_pin);

    return currentData;
}

int c_Joystick::_meanSample(uint8_t sensPin)
{
    int sum = 0;
    int start_t = millis();

    for (int sampleCount = 0; sampleCount < JoystickConstants::operationalConstants::MAX_SAMPLE_COUNT; sampleCount++)
    {
        sum += constrain(analogRead(sensPin), 0, 4095);
        if (millis() - start_t > JoystickConstants::operationalConstants::MAX_SAMPLE_DURATION)
        {
            return int(sum / sampleCount);
        }
    }

    return int(sum / JoystickConstants::operationalConstants::MAX_SAMPLE_COUNT);
}

int c_Joystick::_applyExpo(uint16_t val, int normal_limit)
{
    if (normal_limit <= 0)
        return 0;

    float val_norm = (float)val / normal_limit;
    if (val_norm > 1.0f)
        val_norm = 1.0f;

    float expo_norm = (1 - _expo_factor) * val_norm + _expo_factor * (val_norm * val_norm * val_norm);

    return uint16_t(expo_norm * normal_limit);
}