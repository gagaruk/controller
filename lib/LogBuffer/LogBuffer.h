#pragma once

#include <Arduino.h>
#include "ControllerConfig.h"

class c_logBuffer{
public:
    void push(const String& logMessage);
    void dumpOldest();
    bool isEmpty() const;

private:
    static const size_t _buffer_size = logger::BUFFERS_SIZE;
    String _buffer[_buffer_size];
    size_t _head = 0;
    size_t _tail = 0;
    size_t _count = 0;
};