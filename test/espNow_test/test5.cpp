#include <Arduino.h>
#include <unity.h>
#include "EspNowManager.h"
#include "ControllerConfig.h"

c_EspNowManager espNow;
uint8_t testMacAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

void setUp(void) {}
void tearDown(void) {}

void test_espnow_mac_address(void)
{
    String mac = espNow.getMacAddress();
    TEST_ASSERT_NOT_EQUAL(0, mac.length());
}

void test_espnow_initialization(void)
{
    bool initResult = espNow.init(1, testMacAddress);
    TEST_ASSERT_TRUE_MESSAGE(initResult, "ESP-NOW failed to initialize or register peer");
}

void test_espnow_send_data_oversize(void)
{
    uint8_t largeData[251] = {0};
    // sendData should reject length > 250
    bool sendResult = espNow.sendData(testMacAddress, largeData, 251);
    TEST_ASSERT_FALSE_MESSAGE(sendResult, "ESP-NOW accepted data larger than 250 bytes limit");
}

void test_espnow_receive_data_empty(void)
{
    TelemetryPacket rxPacket;
    // Should return false because _newDataAvailable is default false
    bool rxResult = espNow.recievedData(rxPacket);
    TEST_ASSERT_FALSE_MESSAGE(rxResult, "Reported new data when none was received");
}

void setup()
{
    delay(2000);
    UNITY_BEGIN();
    RUN_TEST(test_espnow_mac_address);
    RUN_TEST(test_espnow_initialization);
    RUN_TEST(test_espnow_send_data_oversize);
    RUN_TEST(test_espnow_receive_data_empty);
    UNITY_END();
}

void loop() {}