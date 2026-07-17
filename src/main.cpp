#include <Arduino.h>
#include "PeripheralManager.h"
#include "ControllerConfig.h"
#include "EspNowManager.h"

c_logBuffer logBuffer;

c_Joystick joy1 = c_Joystick(JoystickConstants::pins::JOY1_X, JoystickConstants::pins::JOY1_Y, JoystickConstants::pins::JOY1_S,
                             JoystickConstants::operationalConstants::DEADBAND, JoystickConstants::operationalConstants::FILTER_ALPHA, JoystickConstants::operationalConstants::EXPO_FACTOR,
                             JoystickConstants::operationalConstants::MAX_SAMPLE_COUNT, JoystickConstants::operationalConstants::MAX_SAMPLE_DURATION);

c_Joystick joy2 = c_Joystick(JoystickConstants::pins::JOY2_X, JoystickConstants::pins::JOY2_Y, JoystickConstants::pins::JOY2_S,
                             JoystickConstants::operationalConstants::DEADBAND, JoystickConstants::operationalConstants::FILTER_ALPHA, JoystickConstants::operationalConstants::EXPO_FACTOR,
                             JoystickConstants::operationalConstants::MAX_SAMPLE_COUNT, JoystickConstants::operationalConstants::MAX_SAMPLE_DURATION);
c_ButtonArray buttonArray = c_ButtonArray(ButtonArray::pins::W, ButtonArray::pins::A, ButtonArray::pins::S, ButtonArray::pins::D, ButtonArray::pins::X);

c_PeripheralManager peripheralManager(&joy1, &joy2, &buttonArray, ButtonArray::pins::toggle);

c_EspNowManager nowTranciever = c_EspNowManager();

PeripheralPacket data;

void setup()
{
    Serial.begin(115200);
    delay(2000);
    peripheralManager.init();
    nowTranciever.init(EspNowConstants::WIFI_CHANNEL, EspNowConstants::BROADCAST_ADDRESS);
}

void loop()
{
    peripheralManager.updateVals();
    peripheralManager.getVals(data);

    nowTranciever.sendData(EspNowConstants::BROADCAST_ADDRESS, &data, sizeof(data));
}