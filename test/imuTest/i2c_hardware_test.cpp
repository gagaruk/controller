#include <Arduino.h>
#include <Wire.h>

// CRITICAL: Change these to the exact physical pins connected to your IMU!
#define SDA_PIN 21
#define SCL_PIN 22

void setup()
{
    Serial.begin(115200);
    while (!Serial)
        ; // Wait for serial monitor to connect
    delay(2000);

    Serial.println("\n=======================================");
    Serial.println("      PHYSICAL I2C HARDWARE SCAN       ");
    Serial.println("=======================================");

    // Initialize the physical I2C wires
    Wire.begin(SDA_PIN, SCL_PIN, 100000);
}

void loop()
{
    byte error, address;
    int devicesFound = 0;

    Serial.println("Scanning I2C bus...");

    for (address = 1; address < 127; address++)
    {
        Wire.beginTransmission(address);
        error = Wire.endTransmission();

        if (error == 0)
        {
            Serial.printf("Found responding chip at address: 0x%02X\n", address);
            devicesFound++;
        }
        else if (error == 4)
        {
            Serial.printf("Unknown error/short circuit at address: 0x%02X\n", address);
        }
    }

    if (devicesFound == 0)
    {
        Serial.println("RESULT: No physical I2C devices detected at all.");
    }
    else
    {
        Serial.println("Scan complete.");
    }

    Serial.println("---------------------------------------");
    delay(4000);
}