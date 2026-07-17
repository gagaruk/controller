#include <Arduino.h>
#include "rfManager.h"

// ==================== CONFIGURATION ====================
#define ROLE_TRANSMITTER true // Change to false for the receiver board

#define NRF_CE_PIN 17
#define NRF_CSN_PIN 5
#define BAUD_RATE 115200
#define TX_INTERVAL 500 // Min interval gap in ms between packets

// Hex-mapped pipeline pathways
const byte ADDR_ALPHA = 0x01;
const byte ADDR_BETA = 0x02;

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

c_RFcommunicator rf(NRF_CE_PIN, NRF_CSN_PIN, BAUD_RATE, TX_INTERVAL);
uint32_t counter = 0;

void setup()
{
    // Note: rf.init() executes internal Serial.begin line matching passed speed configurations
#if ROLE_TRANSMITTER
    Serial.println("\n=== nRF24L01 Transmitter Mode Initialization ===");
    rf.init(ADDR_ALPHA, ADDR_BETA);
#else
    Serial.println("\n=== nRF24L01 Receiver Mode Initialization ===");
    rf.init(ADDR_BETA, ADDR_ALPHA);
#endif
}

void loop()
{
#if ROLE_TRANSMITTER
    // Send data sequence at strict timed hardware update loops
    PeripheralPacket txPacket = {counter++, 1023, -512};
    Serial.printf("Pushing RF Payload Frame ID: %u\n", txPacket.frameId);
    rf.sendPeripheral(txPacket);
    delay(10); // Short cycle break to prevent processor spin lockups

#else
    // Read operations switch state safely into listening configuration profiles
    TelemetryPacket rxPacket;
    if (rf.recieveTelemetry(rxPacket))
    {
        Serial.println("\n--- [Incoming nRF24L01 RF Telemetry Frame] ---");
        Serial.printf("Frame ID Received   : %u\n", rxPacket.frameId);
        Serial.printf("Voltage Metric      : %.2f V\n", rxPacket.batteryVoltage);
        Serial.printf("System Integrity Code: 0x%X\n", rxPacket.statusFlags);
        Serial.println("----------------------------------------------");
    }
#endif
}