#pragma once
#include <Arduino.h>
#include "LogBuffer.h"

extern c_logBuffer logBuffer;

/////////////////////////////////// Logger //////////////////////////////////////////////////////
#define CONTROLLER_DEBUG 1

#if CONTROLLER_DEBUG
    #define _LOG_WRITE(tag, fmt, ...) do { \
        char _local_msg_buf[128]; \
        snprintf(_local_msg_buf, sizeof(_local_msg_buf), "[" tag "] " fmt, ##__VA_ARGS__); \
        logBuffer.push(_local_msg_buf); \
    } while(0)

    #define LOG_INIT(fmt, ...)   _LOG_WRITE("INIT",  fmt, ##__VA_ARGS__)
    #define LOG_INFO(fmt, ...)   _LOG_WRITE("INFO",  fmt, ##__VA_ARGS__)
    #define LOG_ERROR(fmt, ...)  _LOG_WRITE("ERROR", fmt, ##__VA_ARGS__)

    #define LOG_JOYSTICK(x, y)   _LOG_WRITE("JOY",   "X: %d | Y: %d", x, y)
    #define LOG_RADIO(fmt, ...)  _LOG_WRITE("RADIO", fmt, ##__VA_ARGS__)
#else

    #define LOG_INIT(fmt, ...)
    #define LOG_INFO(fmt, ...)
    #define LOG_ERROR(fmt, ...)
    #define LOG_JOYSTICK(x, y)
    #define LOG_RADIO(fmt, ...)
#endif


namespace logger{
    constexpr int BUFFERS_SIZE = 0;
    constexpr int INTERVAL = 0;
}

////////////////////////////////////// Joystick /////////////////////////////////////////////////

typedef struct JoystickData{
    int x_processed = 0;
    int y_processed = 0;
    bool isPressed = false;
};


namespace joystickConstants{
    namespace pins{
        constexpr uint8_t JOY1_X = 0;
        constexpr uint8_t JOY1_Y = 0;
        constexpr uint8_t JOY1_S = 0;
        
        constexpr uint8_t JOY1_X = 0;
        constexpr uint8_t JOY1_Y = 0;
        constexpr uint8_t JOY1_S = 0;
    }

    namespace operationalConstants{
        constexpr int DEADBAND = 0;
        constexpr float FILTER_ALPHA = 0;
        constexpr float EXPO_FACTOR = 0;
        constexpr int maxSampleCount = 0;
        constexpr int maxSampleDuration = 0;
    }


}
