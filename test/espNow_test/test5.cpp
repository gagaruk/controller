#include <Arduino.h>
#include <Wire.h>
#include "EspNowManager.h"

// ==================== CONFIGURATION ====================
#define ROLE_TRANSMITTER true // Change to false for the receiver board
const uint8_t WIFI_CHANNEL = 1;

// PLACEHOLDER PACKETS: Match these to your core definitions
struct PeripheralPacket
{
    uint32_t frameId;
    int16_t joyX;
    int16_t joyY;
};

struct TelemetryPacket
{
    uint32_t frameId;
    float batteryVoltage;
    int32_t statusFlags;
};

// Replace with your peer microcontrollers' physical MAC hardware addresses
const uint8_t TARGET_MAC[6] = {0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF};
// =======================================================

c_EspNowManager espNow;
uint32_t counter = 0;

void setup()
{
    Serial.begin(115200);
    delay(1000);

    Serial.println("\n=== ESP-NOW Verification Test ===");

    // Print local physical address first so you can configure peers easily
    Serial.print("Local Device MAC Address: ");
    Serial.println(espNow.getMacAddress());

    Serial.print("Initializing ESP-NOW Network Stack... ");
    if (espNow.init(WIFI_CHANNEL, TARGET_MAC))
    {
        Serial.println("SUCCESS!");
    }
    else
    {
        Serial.println("FAILED! System Halted.");
        while (true)
            delay(10);
    }
}

void loop()
{
#if ROLE_TRANSMITTER
    // Transmitter Mode: Package data payload up to 250 byte buffer limit
    PeripheralPacket txPacket = {counter++, 512, 512};

    Serial.printf("Transmitting Packet Frame: %u\n", txPacket.frameId);
    bool success = espNow.sendData(TARGET_MAC, (const uint8_t *)&txPacket, sizeof(txPacket));

    if (!success)
    {
        Serial.println("Execution Error: Target send rejected.");
    }
    delay(1000);

#else
    // Receiver Mode: Continually process background thread-safe packet queues
    TelemetryPacket rxPacket;
    if (espNow.recievedData(rxPacket))
    {
        Serial.println("\n--- [Incoming ESP-NOW Telemetry Event] ---");
        Serial.printf("Frame ID Received : %u\n", rxPacket.frameId);
        Serial.printf("Battery Metric    : %.2f V\n", rxPacket.batteryVoltage);
        Serial.printf("Status Code Register: 0x%X\n", rxPacket.statusFlags);
        Serial.println("------------------------------------------");
    }
#endif
}