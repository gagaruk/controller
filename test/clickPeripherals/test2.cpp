#include <Arduino.h>
#include "PeripheralManager.h"
#include "ControllerConfig.h"

c_logBuffer logBuffer;

c_Joystick joy1 = c_Joystick(JoystickConstants::pins::JOY1_X, JoystickConstants::pins::JOY1_Y, JoystickConstants::pins::JOY1_S,
                             JoystickConstants::operationalConstants::DEADBAND, JoystickConstants::operationalConstants::FILTER_ALPHA, JoystickConstants::operationalConstants::EXPO_FACTOR,
                             JoystickConstants::operationalConstants::MAX_SAMPLE_COUNT, JoystickConstants::operationalConstants::MAX_SAMPLE_DURATION);

c_Joystick joy2 = c_Joystick(JoystickConstants::pins::JOY2_X, JoystickConstants::pins::JOY2_Y, JoystickConstants::pins::JOY2_S,
                             JoystickConstants::operationalConstants::DEADBAND, JoystickConstants::operationalConstants::FILTER_ALPHA, JoystickConstants::operationalConstants::EXPO_FACTOR,
                             JoystickConstants::operationalConstants::MAX_SAMPLE_COUNT, JoystickConstants::operationalConstants::MAX_SAMPLE_DURATION);
c_ButtonArray buttonArray = c_ButtonArray(ButtonArray::pins::W, ButtonArray::pins::A, ButtonArray::pins::S, ButtonArray::pins::D, ButtonArray::pins::X);

c_PeripheralManager peripheralManager(&joy1, &joy2, &buttonArray, ButtonArray::pins::toggle);

PeripheralPacket data;

void setup()
{
    Serial.begin(115200);
    delay(2000); // Give the serial monitor time to connect

    // Initialize your logging system if it requires setup (e.g., logBuffer.init())
    // logBuffer.init();

    peripheralManager.init(); // Fire up hardware pins
    Serial.println("--- Peripherals Initialized. Streaming Live Data Below ---");
}

void loop()
{

    // This single line updates the hardware and triggers your internal LOG_* macros![cite: 1]
    peripheralManager.updateVals();
    peripheralManager.getVals(data);

    Serial.printf("J1: %d,  %d,  %D | J2: %d,  %d,  %d| ButtonArray: %d%d%d%d%d | Toggle: %s", data.joy1.x, data.joy1.y, data.joy1.sw,
                  data.joy2.x, data.joy2.y, data.joy2.sw,
                  data.buttArr.w, data.buttArr.a, data.buttArr.s, data.buttArr.d, data.buttArr.x,
                  (data.switchState) ? "ON" : "OFF");
    Serial.println();

    delay(200); // Control the scroll speed of your monitor
}