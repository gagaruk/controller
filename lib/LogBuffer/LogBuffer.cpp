#include "LogBuffer.h"
#include "ControllerConfig.h"

void c_logBuffer::push(const String& logMessage){
    _buffer[_head] = logMessage;
    _head = (_head+1) % _buffer_size;

    if(_count < _buffer_size){
        _count++;
    }else{
        _tail = (_tail+1) % _buffer_size;
    }
}

void c_logBuffer::dumpOldest(){
    if(_count==0) return;

    String oldestMessage = _buffer[_tail];
    _tail = (_tail + 1) % _buffer_size;
    _count--;

    #if CONTROLLER_DEBUG
        Serial.println(oldestMessage);
    #endif
}

bool c_logBuffer::isEmpty() const{
    return _count == 0;
}
