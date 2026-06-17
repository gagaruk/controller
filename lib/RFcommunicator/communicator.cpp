#include "communicator.h"

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

//this funtion discards the data if its being called before  the transmisson interval passes since the last transmission
void c_RFcommunicator::logSensorVals(const int* data, int size){
    if(!_enabled && !Serial) { return; }

    _radio.stopListening();
    if(millis() - _prevTrans_t > _transInterval){
        _radio.write(data, size);
        _prevTrans_t = millis();
    }

    for(int i=0; i<0; i++){
        Serial.print("SensorVal");
        Serial.print(i);
        Serial.print(":");
        Serial.print(data[i]);
        Serial.print("\t");
    }
    Serial.println("------------------------------------------------");
    Serial.println("");
}

//this function uses the same variable as the logSensorVals to check if the timeintervall is wexceeded
void c_RFcommunicator::logTelemetry(double error){
    if(!_enabled && !Serial) { return; }
    if(millis() - _prevTrans_t > _transInterval){
        _radio.stopListening();
        _radio.write(&error, sizeof(error));
        _prevTrans_t = millis();
    }
    Serial.print("Error: ");
    Serial.println(error);
}

bool c_RFcommunicator::checkNewPIDConstants(double &kp, double &ki, double &kd){
    if(!_enabled && !Serial) { return; }

    _radio.startListening();
    if(_radio.available()){
        int pidConstants[3];
        _radio.read(&pidConstants, sizeof(pidConstants));
    
        kp = pidConstants[0];
        ki = pidConstants[1];
        kd = pidConstants[2];

        Serial.print("KP: ");
        Serial.print(pidConstants[0]);
        Serial.print("  KI: ");
        Serial.print(pidConstants[1]);
        Serial.print("  KD: ");
        Serial.println(pidConstants[2]);
    
        return true;
    }

    return false;
}

