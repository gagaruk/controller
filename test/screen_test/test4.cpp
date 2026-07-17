#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ControllerConfig.h>

Adafruit_SSD1306 display(displayConstants::SCREEN_WIDTH, displayConstants::SCREEN_HEIGHT, &Wire, -1);

void setup()
{
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(10, 10);
    display.println("Hello");
    display.display();
}

void loop()
{
}