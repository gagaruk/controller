#include <Arduino.h>
#include <unity.h> // Required for PlatformIO testing

void test_serial_message(void)
{
    // Print your sanity check string
    Serial.println("\n[TEST] If this prints, serial communications are working perfectly!");

    // Unity requires at least one assertion to pass a test
    TEST_ASSERT_TRUE(true);
}

void setup()
{
    // Crucial: Give the ESP32 and serial monitor 2 seconds to align before printing
    delay(2000);

    UNITY_BEGIN();
    RUN_TEST(test_serial_message);
    UNITY_END();
}

void loop()
{
    // Keep this empty! Unity handles the exit.
}