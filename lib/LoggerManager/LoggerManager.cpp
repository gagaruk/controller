#include "LogBuffer.h"
#include "LoggerManager.h"


c_loggerManager::c_loggerManager(c_logBuffer& buffer, int interval):
    _buffer(buffer), _interval(interval){}

void c_loggerManager::process(){
    if(millis() - _lastLogTime){
        if(!_buffer.isEmpty()){
            _buffer.dumpOldest();
        }
        _lastLogTime = millis();
    }
}

void c_loggerManager::forceEmergencyDump(){
    while(!_buffer.isEmpty()){
        _buffer.dumpOldest();
    }
    _lastLogTime = millis();
}