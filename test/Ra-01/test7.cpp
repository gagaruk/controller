#include <Arduino.h>
#include "LoRaManager.h"

// ==================== CONFIGURATION ====================
#define ROLE_TRANSMITTER true // Change to false for the receiver board

#define LORA_CS_PIN 15
#define LORA_RST_PIN 14
#define LORA_IRQ_PIN 2
#define LORA_FREQ_HZ 433E6

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
// =======================================================

c_RadioManager lora(LORA_CS_PIN, LORA_RST_PIN, LORA_IRQ_PIN, LORA_FREQ_HZ);
uint32_t counter = 0;

void setup()
{
    Serial.begin(115200);
    delay(1000);
    Serial.println("\n=== Ra-01 LoRa Initialization Pipeline ===");

    if (lora.begin())
    {
        Serial.println("LoRa Transceiver State: ONLINE");
    }
    else
    {
        Serial.println("LoRa Transceiver State: CRITICAL INIT FAILURE");
        while (true)
            delay(10);
    }
}

void loop()
{
#if ROLE_TRANSMITTER
    PeripheralPacket txPacket = {counter++, 2048, 2048};
    Serial.printf("Broadcasting LoRa Air Frame: %u\n", txPacket.frameId);

    if (!lora.transmitPacket(txPacket))
    {
        Serial.println("Air-Traffic Warning: Packet collision or tracking failure.");
    }
    delay(2000); // LoRa requires higher airtime; enforce larger operational gaps

#else
    TelemetryPacket rxPacket;
    if (lora.recievePacket(rxPacket))
    {
        Serial.println("\n--- [Incoming Ra-01 LoRa RF Signal Event] ---");
        Serial.printf("Frame ID Decoded   : %u\n", rxPacket.frameId);
        Serial.printf("Sensor Voltage Read: %.2f V\n", rxPacket.batteryVoltage);
        Serial.printf("System Status Flags: 0x%X\n", rxPacket.statusFlags);
        Serial.println("--------------------------------------------");
    }
#endif
}