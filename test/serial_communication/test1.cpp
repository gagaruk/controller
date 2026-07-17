#include <Arduino.h>

long testCounter = 0;
void setup()

{
    Serial.begin(115200);
}

void loop()
{
    Serial.print("Sensor Readout Loop #");
    Serial.println(testCounter);

    testCounter++;

    delay(200);
}