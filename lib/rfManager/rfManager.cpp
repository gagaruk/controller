#include "rfManager.h"

c_RFcommunicator::c_RFcommunicator(uint8_t ce, uint8_t cns, int baudrate, int transInterval) : _radio(ce,cns), _baudrate(baudrate), _transInterval(transInterval){
    _prevTrans_t = 0;
}

void c_RFcommunicator::init(byte writingAddress, byte readingAddress){
    Serial.begin(_baudrate);
    _enabled = true;
    _radio.begin();
    _radio.openWritingPipe(writingAddress);
    _radio.openReadingPipe(1,readingAddress);
    _radio.setChannel(76);
    _radio.setDataRate(RF24_250KBPS);
    _radio.setPALevel(RF24_PA_MAX);
}

void c_RFcommunicator::disableComms(){
    _enabled = false;
}

//this function uses the same variable as the sendPeripheral to check if the timeintervall is wexceeded
void c_RFcommunicator::sendPeripheral(const PeripheralPacket& packet){
    if(!_enabled && !Serial) { return; }
    if(millis() - _prevTrans_t > _transInterval){
        _radio.stopListening();
        _radio.write(&packet, sizeof(packet));
        _prevTrans_t = millis();
    }
}

bool c_RFcommunicator::recieveTelemetry(TelemetryPacket &packet){
    if(!_enabled && !Serial) { return; }

    _radio.startListening();
    if(_radio.available()){

        _radio.read(&packet, sizeof(packet));
    
        return true;
    }

    return false;
}

