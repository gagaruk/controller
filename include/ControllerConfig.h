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

    #define LOG_INIT(fmt, ...) do {} while(0)
    #define LOG_INFO(fmt, ...) do {} while(0)
    #define LOG_ERROR(fmt, ...) do {} while(0)
    #define LOG_JOYSTICK(x, y) do {} while(0)
    #define LOG_RADIO(fmt, ...) do {} while(0)
#endif


namespace Loggerconstants{
    constexpr int BUFFERS_SIZE = 0;
    constexpr int INTERVAL = 0;
}

////////////////////////////////////// Joystick /////////////////////////////////////////////////

typedef struct JoystickData{
    int x_processed = 0;
    int y_processed = 0;
    bool isPressed = false;
};


namespace JoystickConstants{
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
        constexpr int MAX_SAMPLE_COUNT = 0;
        constexpr int MAX_SAMPLE_DURATION = 0;
    }
}

//////////////////////////////////////////// Serial Coms ////////////////////////////////////////////////////////

struct OdometryData{
    double gyro_x;
    double gyro_y;
    double gyro_z;

    double accel_x;
    double accel_y;
    double accel_z;

    double height;
    double heading;
};

struct SystemStatus{
    float batteryVoltage;
    uint8_t rssi;
    uint8_t errorCode;
};

typedef struct TelemetryPacket{
    OdometryData odometry;
    SystemStatus status;
}__attribute__((packed));


namespace LoRaConstants{
    namespace pins{
        constexpr uint8_t CS = 0;
        constexpr uint8_t RESET = 0;
        constexpr uint8_t IRQ = 0;
    }
    namespace operationalConstants{
        constexpr int SPREADING_FACTOR = 7;
        constexpr long SIGNAL_BANDWITH = 250E3;
        constexpr int CODING_RATE = 5;
        constexpr int FREQ = 0;
    }
}

namespace EspNowConstants{
    constexpr uint8_t BROADCAST_ADDRESS[] = {0xE4, 0x65, 0xB8, 0xD8, 0x9C, 0x60};
    constexpr int WIFI_CHANNEL = 1;
    constexpr wifi_interface_t WIFI_INTERFACE = WIFI_IF_STA;
    constexpr uint8_t WIFI_PROTOCOL = WIFI_PROTOCOL_LR;
    constexpr wifi_power_t TX_POWER = WIFI_POWER_19_5dBm;
}