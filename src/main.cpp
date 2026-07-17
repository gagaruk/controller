#include "ControllerConfig.h"

#include "Joystick.h"
#include "ButtonArray.h"
#include "PeripheralManager.h"
#include "EspNowManager.h"

c_logBuffer logBuffer;

c_Joystick joy1 = c_Joystick(JoystickConstants::pins::JOY1_X, JoystickConstants::pins::JOY1_Y, JoystickConstants::pins::JOY1_S,
                             JoystickConstants::operationalConstants::DEADBAND, JoystickConstants::operationalConstants::FILTER_ALPHA, JoystickConstants::operationalConstants::EXPO_FACTOR,
                             JoystickConstants::operationalConstants::MAX_SAMPLE_COUNT, JoystickConstants::operationalConstants::MAX_SAMPLE_DURATION);

c_Joystick joy2 = c_Joystick(JoystickConstants::pins::JOY2_X, JoystickConstants::pins::JOY2_Y, JoystickConstants::pins::JOY2_S,
                             JoystickConstants::operationalConstants::DEADBAND, JoystickConstants::operationalConstants::FILTER_ALPHA, JoystickConstants::operationalConstants::EXPO_FACTOR,
                             JoystickConstants::operationalConstants::MAX_SAMPLE_COUNT, JoystickConstants::operationalConstants::MAX_SAMPLE_DURATION);

c_ButtonArray buttons = c_ButtonArray(ButtonArray::pins::W, ButtonArray::pins::A, ButtonArray::pins::S, ButtonArray::pins::D, ButtonArray::pins::X);

c_PeripheralManager peripheralManager = c_PeripheralManager(&joy1, &joy2, &buttons, ButtonArray::pins::toggle);

c_EspNowManager espNowManager = c_EspNowManager();

PeripheralPacket peripheralPacket;
TelemetryPacket telemetryPacket;

void setup()
{
    Serial.begin(115200);
    peripheralManager.init();
}

void loop()
{
    peripheralManager.updateVals();
    peripheralManager.getVals(peripheralPacket);

    espNowManager.sendData(EspNowConstants::BROADCAST_ADDRESS, (const uint8_t *)&peripheralPacket, sizeof(peripheralPacket));
}