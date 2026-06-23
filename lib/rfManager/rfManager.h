#pragma once

#include "ControllerConfig.h"

#include <RF24.h>
#include <nRF24L01.h>

class c_RFcommunicator{
public:
    c_RFcommunicator(uint8_t ce, uint8_t cns, int baudrate, int transInterval);
    void init(byte writingAddress, byte readingAddress);
    void disableComms();

    void sendPeripheral(const PeripheralPacket& packet);
    bool recieveTelemetry(TelemetryPacket &packet);
    
private:
    RF24 _radio;
    uint8_t _ce, _cns;
    int _baudrate;
    int _transInterval;
    bool _enabled = false;
    unsigned long _prevTrans_t = 0;
};
