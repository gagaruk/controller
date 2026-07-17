#include <Arduino.h>

void setup()
{
    Serial.begin(115200);
}
void loop()
{
    Serial.println("If this prints the serial communications are working");
}
