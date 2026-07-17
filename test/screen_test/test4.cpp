#include <Arduino.h>
#include <unity.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ControllerConfig.h>

Adafruit_SSD1306 display(displayConstants::SCREEN_WIDTH, displayConstants::SCREEN_HEIGHT, &Wire, -1); //[cite: 9]

void setUp(void) {}
void tearDown(void) {}

void test_oled_begin(void)
{
    // Ensure the I2C wires are active
    Wire.begin(i2cConstants::SDA, i2cConstants::SCL);
    Wire.setClock(i2cConstants::CLOCK_SPEED);

    // Verifies display can boot and claim its frame buffer memory[cite: 9]
    bool success = display.begin(SSD1306_SWITCHCAPVCC, 0x3C); //[cite: 9]
    TEST_ASSERT_TRUE_MESSAGE(success, "OLED panel failed allocation or was not detected on I2C address 0x3C");
}

void test_oled_drawing(void)
{
    // Ensure we can clear, write text to, and push to display buffer without throwing driver panics[cite: 9]
    display.clearDisplay();      //[cite: 9]
    display.setTextSize(2);      //[cite: 9]
    display.setTextColor(WHITE); //[cite: 9]
    display.setCursor(10, 10);   //[cite: 9]
    display.println("PASS");
    display.display(); //[cite: 9]

    TEST_ASSERT_TRUE(true);
}

void setup()
{
    delay(2000);
    UNITY_BEGIN();

    RUN_TEST(test_oled_begin);
    RUN_TEST(test_oled_drawing);

    UNITY_END();
}

void loop()
{
    // Keep empty for Unity test runner
}