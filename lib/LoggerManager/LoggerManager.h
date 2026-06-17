#pragma once
#include <Arduino.h>
#include "LogBuffer.h"

class c_loggerManager{
public:
    c_loggerManager(c_logBuffer& buffer, int interval);
    void process();
    void forceEmergencyDump();

private:
    c_logBuffer& _buffer;
    unsigned long _lastLogTime = 0;
    unsigned long _interval = 0;
};


