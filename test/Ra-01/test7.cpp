#include <Arduino.h>
#include <unity.h>
#include "LoRaManager.h"

// Define test pins and frequency
const uint8_t TEST_CS_PIN = 5;
const uint8_t TEST_RST_PIN = 14;
const uint8_t TEST_IRQ_PIN = 2;
const long TEST_FREQ = 433E6;

c_RadioManager lora(TEST_CS_PIN, TEST_RST_PIN, TEST_IRQ_PIN, TEST_FREQ);

void setUp(void)
{
    // Runs before each test
}

void tearDown(void)
{
    // Runs after each test
}

void test_lora_initialization(void)
{
    bool initResult = lora.begin();
    TEST_ASSERT_TRUE_MESSAGE(initResult, "LoRa Radio failed to initialize at 433MHz");
}

void test_lora_transmit_packet(void)
{
    PeripheralPacket testPacket = {}; // Assuming default constructor/zero initialization
    bool txResult = lora.transmitPacket(testPacket);
    TEST_ASSERT_TRUE_MESSAGE(txResult, "Radio transmission failed to clear or buffer blocked");
}

void test_lora_receive_packet(void)
{
    TelemetryPacket testPacket;
    // Note: This will return false unless a physical packet is currently in the air/buffer
    // For a true unit test, mocking LoRa.parsePacket() is required.
    bool rxResult = lora.recievePacket(testPacket);
    TEST_ASSERT_FALSE_MESSAGE(rxResult, "Expected no packet without external transmission");
}

void setup()
{
    delay(2000);
    UNITY_BEGIN();
    RUN_TEST(test_lora_initialization);
    RUN_TEST(test_lora_transmit_packet);
    RUN_TEST(test_lora_receive_packet);
    UNITY_END();
}

void loop() {}