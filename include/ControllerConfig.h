#pragma once
#include <Arduino.h>
#include <WiFi.h>

/////////////////////////////////// Logger //////////////////////////////////////////////////////
#define CONTROLLER_DEBUG 1

#if CONTROLLER_DEBUG
#define _LOG_WRITE(tag, fmt, ...)                                                          \
    do                                                                                     \
    {                                                                                      \
        char _local_msg_buf[128];                                                          \
        snprintf(_local_msg_buf, sizeof(_local_msg_buf), "[" tag "] " fmt, ##__VA_ARGS__); \
        logBuffer.push(_local_msg_buf);                                                    \
    } while (0)

#define LOG_INIT(fmt, ...) _LOG_WRITE("INIT", fmt, ##__VA_ARGS__)
#define LOG_INFO(fmt, ...) _LOG_WRITE("INFO", fmt, ##__VA_ARGS__)
#define LOG_ERROR(fmt, ...) _LOG_WRITE("ERROR", fmt, ##__VA_ARGS__)

#define LOG_JOYSTICK(x, y) _LOG_WRITE("JOY", "X: %d | Y: %d", x, y)
#define LOG_BUTTONS(W, A, S, D, X) _LOG_WRITE("BUTTONS", "W: %d | A: %d | S: %d | D: %d | X: %d", W, A, S, D, X)
#define LOG_SWITCH(state) _LOG_WRITE("SWITCH", "State: %d", state)
#define LOG_RADIO(fmt, ...) _LOG_WRITE("RADIO", fmt, ##__VA_ARGS__)
#define LOG_TELEMETRY(gyro_x, gyro_y, gyro_z, accel_x, accel_y, accel_z, height, heading, batteryVoltage, rssi, errorCode)                                     \
    _LOG_WRITE("TELEMETRY", "Gyro: [%.2f, %.2f, %.2f] | Accel: [%.2f, %.2f, %.2f] | Height: %.2f | Heading: %.2f | Battery: %.2f | RSSI: %d | Error Code: %d", \
               gyro_x, gyro_y, gyro_z, accel_x, accel_y, accel_z, height, heading, batteryVoltage, rssi, errorCode)
#else

#define LOG_INIT(fmt, ...) \
    do                     \
    {                      \
    } while (0)
#define LOG_INFO(fmt, ...) \
    do                     \
    {                      \
    } while (0)
#define LOG_ERROR(fmt, ...) \
    do                      \
    {                       \
    } while (0)
#define LOG_JOYSTICK(x, y) \
    do                     \
    {                      \
    } while (0)
#define LOG_BUTTONS(w, a, s, d, x) \
    do                             \
    {                              \
    } while (0)
#define LOG_SWITCH(state) \
    do                    \
    {                     \
    } while (0)
#define LOG_RADIO(fmt, ...) \
    do                      \
    {                       \
    } while (0)
#define LOG_TELEMETRY(gyro_x, gyro_y, gyro_z, accel_x, accel_y, accel_z, height, heading, batteryVoltage, rssi, errorCode) \
    do                                                                                                                     \
    {                                                                                                                      \
    } while (0)
#endif

namespace LoggerConstants
{
    static constexpr int BUFFER_SIZE = 15;
}
#include "LogBuffer.h"
extern c_logBuffer logBuffer;
////////////////////////////////////// Joystick /////////////////////////////////////////////////

struct JoystickData
{
    int x = 0;
    int y = 0;
    bool sw = false;
};

namespace JoystickConstants
{
    namespace pins
    {
        constexpr uint8_t JOY1_X = 36;
        constexpr uint8_t JOY1_Y = 39;
        constexpr uint8_t JOY1_S = 32;

        constexpr uint8_t JOY2_X = 34;
        constexpr uint8_t JOY2_Y = 35;
        constexpr uint8_t JOY2_S = 33;
    }

    namespace operationalConstants
    {
        constexpr int DEADBAND = 10;
        constexpr float FILTER_ALPHA = 0.1;
        constexpr float EXPO_FACTOR = 0.2;
        constexpr int MAX_SAMPLE_COUNT = 16;
        constexpr int MAX_SAMPLE_DURATION = 50;
    }
}

//////////////////////////////////////////// Serial Coms ////////////////////////////////////////////////////////

struct OdometryData
{
    double gyro_x;
    double gyro_y;
    double gyro_z;

    double accel_x;
    double accel_y;
    double accel_z;

    double height;
    double heading;
};

struct SystemStatus
{
    float batteryVoltage;
    uint8_t rssi;
    uint8_t errorCode;
};

struct TelemetryPacket
{
    OdometryData odometry;
    SystemStatus status;
};

namespace LoRaConstants
{
    namespace pins
    {
        constexpr uint8_t CS = 0;
        constexpr uint8_t RESET = 0;
        constexpr uint8_t IRQ = 0;
    }
    namespace operationalConstants
    {
        constexpr int SPREADING_FACTOR = 7;
        constexpr long SIGNAL_BANDWITH = 250E3;
        constexpr int CODING_RATE = 5;
        constexpr int FREQ = 0;
    }
}

namespace EspNowConstants
{
    constexpr uint8_t BROADCAST_ADDRESS[] = {0xE4, 0x65, 0xB8, 0xD8, 0x9C, 0x60};
    constexpr int WIFI_CHANNEL = 1;
    constexpr wifi_interface_t WIFI_INTERFACE = WIFI_IF_STA;
    constexpr uint8_t WIFI_PROTOCOL = WIFI_PROTOCOL_LR;
    constexpr wifi_power_t TX_POWER = WIFI_POWER_19_5dBm;
}

namespace ButtonArray
{
    namespace pins
    {
        constexpr uint8_t W = 26;
        constexpr uint8_t A = 25;
        constexpr uint8_t S = 27;
        constexpr uint8_t D = 14;
        constexpr uint8_t X = 13;
        constexpr uint8_t toggle = 16;
    }
    namespace masks
    {
        constexpr uint8_t W = 1 << 0;
        constexpr uint8_t A = 1 << 1;
        constexpr uint8_t S = 1 << 2;
        constexpr uint8_t D = 1 << 3;
        constexpr uint8_t X = 1 << 4;
    }
    namespace pressedStates
    {
        constexpr bool W = HIGH;
        constexpr bool A = HIGH;
        constexpr bool S = HIGH;
        constexpr bool D = HIGH;
        constexpr bool X = HIGH;
    }

    constexpr int DEBOUNCE_DELAY = 10;
}

struct ButtonData
{
    bool w;
    bool a;
    bool s;
    bool d;
    bool x;
};

namespace imuConstants
{
    static constexpr uint8_t MPU_ADDR = 0x68;

    static constexpr uint8_t POWER_ADDR = 0x6B;
    static constexpr uint8_t MAG_ADDR = 0x1E;
    static constexpr uint8_t CONFIG_ADDR = 0x1A;

    enum class GYRO_RANGE
    {
        DEG_250,
        DEG_500,
        DEG_1000,
        DEG_2000
    };
    enum class ACCEL_RANGE
    {
        G_2,
        G_4,
        G_8,
        G_16
    };
    enum class DLPF_FREQ
    {
        HZ_256,
        HZ_188,
        HZ_98,
        HZ_42,
        HZ_20,
        HZ_10,
        HZ_5
    };
}

struct vector3
{
    float x;
    float y;
    float z;
};

struct IMUData
{
    vector3 gyro;
    vector3 accel;
    vector3 mag;
};

struct PeripheralPacket
{
    JoystickData joy1;
    JoystickData joy2;
    ButtonData buttArr;
    bool switchState;
    IMUData imuData;
};

namespace i2cConstants
{
    static constexpr uint8_t SDA = 21;
    static constexpr uint8_t SCL = 22;
    static constexpr unsigned long CLOCK_SPEED = 400000;
}

namespace loopConstants
{
    static constexpr int FAST_LOOP_TIME_us = 10000;
    static constexpr int SLOW_LOOP_TIME_us = 50000;
}

namespace displayConstants
{
    static constexpr int SCREEN_WIDTH = 128;
    static constexpr int SCREEN_HEIGHT = 64;
}
