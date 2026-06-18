#pragma once

#include <Arduino.h>
#include "ControllerConfig.h"

class c_RadioManager{
public:
    c_RadioManager(uint8_t csPin, uint8_t resetPin, uint8_t irqPin, long freq);
    bool begin();
    bool transmitPacket(const JoystickData& packet);
    bool recievePacket(TelemetryPacket& data);
private:
    uint8_t _csPin = 0;
    uint8_t _resetPin = 0;
    uint8_t _irqPin = 0;
    long _freq;
};