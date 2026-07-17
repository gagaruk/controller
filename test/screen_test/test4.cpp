#include <Wire.h>
#include "OledDisplay.h"
#include "PeripheralManager.h"
#include "ControllerConfig.h"

c_screen display = c_screen(Wire, displayConstants::REFRESH_INTERVAL_MS);

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
    delay(2000);

    peripheralManager.init();
    Serial.println("--- Peripherals Initialized. Streaming Live Data Below ---");
    display.init();
}

void loop()
{

    peripheralManager.updateVals();
    peripheralManager.getVals(data);

    display.update_peripheral_data(data);
    display.display_peripheral();
}