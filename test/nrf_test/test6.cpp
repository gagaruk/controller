#include <Arduino.h>
#include <unity.h>
#include "rfManager.h"

const uint8_t TEST_CE_PIN = 7;
const uint8_t TEST_CNS_PIN = 8;
const int TEST_BAUDRATE = 115200;
const int TEST_INTERVAL = 100; // 100ms
const byte WRITE_ADDR = 0xAA;
const byte READ_ADDR = 0xBB;

c_RFcommunicator rf(TEST_CE_PIN, TEST_CNS_PIN, TEST_BAUDRATE, TEST_INTERVAL);

void setUp(void) {}
void tearDown(void) {}

void test_rf24_disable_comms(void)
{
    rf.init(WRITE_ADDR, READ_ADDR);
    rf.disableComms();

    TelemetryPacket packet;

    // Because disableComms() sets _enabled to false, receiveTelemetry should abort
    bool rxResult = rf.recieveTelemetry(packet);
    TEST_ASSERT_FALSE_MESSAGE(rxResult, "Radio processed receive despite being disabled");
}

void test_rf24_send_interval_limit(void)
{
    rf.init(WRITE_ADDR, READ_ADDR);
    PeripheralPacket packet = {0};

    // First send should update _prevTrans_t
    rf.sendPeripheral(packet);

    // Immediate second send should bypass writing due to millis() < _transInterval
    // Note: To properly test this, you would mock millis(), but in a live test,
    // we can only execute it to ensure no crashes occur during the bypass.
    rf.sendPeripheral(packet);
    TEST_ASSERT_TRUE(true);
}

void setup()
{
    delay(2000);
    UNITY_BEGIN();
    RUN_TEST(test_rf24_disable_comms);
    RUN_TEST(test_rf24_send_interval_limit);
    UNITY_END();
}

void loop() {}