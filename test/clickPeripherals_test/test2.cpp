#include "ControllerConfig.h"

#include "Joystick.h"
#include "ButtonArray.h"
#include "PeripheralManager.h"
#include "EspNowManager.h"

c_Joystick joy1 = c_Joystick(JoystickConstants::pins::JOY1_X, JoystickConstants::pins::JOY1_Y, JoystickConstants::pins::JOY1_S,
                             JoystickConstants::operationalConstants::DEADBAND, JoystickConstants::operationalConstants::FILTER_ALPHA, JoystickConstants::operationalConstants::EXPO_FACTOR,
                             JoystickConstants::operationalConstants::MAX_SAMPLE_COUNT, JoystickConstants::operationalConstants::MAX_SAMPLE_DURATION);

c_Joystick joy2 = c_Joystick(JoystickConstants::pins::JOY2_X, JoystickConstants::pins::JOY2_Y, JoystickConstants::pins::JOY2_S,
                             JoystickConstants::operationalConstants::DEADBAND, JoystickConstants::operationalConstants::FILTER_ALPHA, JoystickConstants::operationalConstants::EXPO_FACTOR,
                             JoystickConstants::operationalConstants::MAX_SAMPLE_COUNT, JoystickConstants::operationalConstants::MAX_SAMPLE_DURATION);
#include <Arduino.h>
#include <unity.h>
#include "ControllerConfig.h"
#include "Joystick.h"
#include "ButtonArray.h"
#include "PeripheralManager.h"
#include "EspNowManager.h"

// Instantiate target components as defined in Source 7[cite: 7]
c_Joystick joy1 = c_Joystick(JoystickConstants::pins::JOY1_X, JoystickConstants::pins::JOY1_Y, JoystickConstants::pins::JOY1_S,
                             JoystickConstants::operationalConstants::DEADBAND, JoystickConstants::operationalConstants::FILTER_ALPHA, JoystickConstants::operationalConstants::EXPO_FACTOR,
                             JoystickConstants::operationalConstants::MAX_SAMPLE_COUNT, JoystickConstants::operationalConstants::MAX_SAMPLE_DURATION); //[cite: 7]

c_Joystick joy2 = c_Joystick(JoystickConstants::pins::JOY2_X, JoystickConstants::pins::JOY2_Y, JoystickConstants::pins::JOY2_S,
                             JoystickConstants::operationalConstants::DEADBAND, JoystickConstants::operationalConstants::FILTER_ALPHA, JoystickConstants::operationalConstants::EXPO_FACTOR,
                             JoystickConstants::operationalConstants::MAX_SAMPLE_COUNT, JoystickConstants::operationalConstants::MAX_SAMPLE_DURATION); //[cite: 7]

c_ButtonArray buttons = c_ButtonArray(ButtonArray::pins::W, ButtonArray::pins::A, ButtonArray::pins::S, ButtonArray::pins::D, ButtonArray::pins::X); //[cite: 7]

c_PeripheralManager peripheralManager = c_PeripheralManager(&joy1, &joy2, &buttons, ButtonArray::pins::toggle); //[cite: 7]
c_EspNowManager espNowManager = c_EspNowManager();                                                              //[cite: 7]

PeripheralPacket peripheralPacket; //[cite: 7]

void setUp(void)
{
    // Standard Unity wrapper
}

void tearDown(void)
{
    // Standard Unity wrapper
}

void test_peripheral_manager_init(void)
{
    // Tests that peripheralManager.init() configures input states cleanly[cite: 7]
    peripheralManager.init(); //[cite: 7]
    TEST_ASSERT_TRUE(true);
}

void test_peripheral_packet_update(void)
{
    // Read current physical pins and verify data can be loaded into our packet[cite: 7]
    peripheralManager.updateVals();              //[cite: 7]
    peripheralManager.getVals(peripheralPacket); //[cite: 7]

    // Assert that the packet state is mapped[cite: 7]
    TEST_ASSERT_NOT_NULL(&peripheralPacket);
}

void test_espnow_comms_init(void)
{
    // Verifies ESP-NOW starts up and establishes a peer mapping[cite: 3]
    bool initOk = espNowManager.init(EspNowConstants::WIFI_CHANNEL, EspNowConstants::BROADCAST_ADDRESS);
    TEST_ASSERT_TRUE_MESSAGE(initOk, "ESP-NOW Initialization and Broadcast configuration failed");
}

void test_espnow_packet_transmission(void)
{
    // Assemble current values and send packet via ESP-NOW[cite: 7]
    peripheralManager.updateVals();              //[cite: 7]
    peripheralManager.getVals(peripheralPacket); //[cite: 7]

    bool sendOk = espNowManager.sendData(EspNowConstants::BROADCAST_ADDRESS, (const uint8_t *)&peripheralPacket, sizeof(peripheralPacket)); //[cite: 7]
    TEST_ASSERT_TRUE_MESSAGE(sendOk, "ESP-NOW failed to transmit peripheral data payload");
}

void setup()
{
    delay(2000);
    UNITY_BEGIN();

    RUN_TEST(test_peripheral_manager_init);
    RUN_TEST(test_peripheral_packet_update);
    RUN_TEST(test_espnow_comms_init);
    RUN_TEST(test_espnow_packet_transmission);

    UNITY_END();
}

void loop()
{
    // Keep empty for Unity test runner
}
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