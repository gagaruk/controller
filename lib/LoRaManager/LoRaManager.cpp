#include "LoRaManager.h"
#include "ControllerConfig.h"
#include <LoRa.h>


c_RadioManager::c_RadioManager(uint8_t csPin, uint8_t resetPin, uint8_t irqPin, long freq):
    _csPin(csPin), _resetPin(resetPin), _irqPin(irqPin), _freq(freq){}


bool c_RadioManager::begin(){
    LoRa.setPins(_csPin, _resetPin, _irqPin);
    
    if(!LoRa.begin(_freq)){
        LOG_ERROR("Ra-01 LoRa Radio failed to initialize!");
        return false;
    }

    LoRa.setSpreadingFactor(LoRaConstants::operationalConstants::SPREADING_FACTOR);
    LoRa.setSignalBandwidth(LoRaConstants::operationalConstants::SIGNAL_BANDWITH);
    LoRa.setCodingRate4(LoRaConstants::operationalConstants::CODING_RATE);

    LOG_INIT("Ra-01 Radio operational at 433MHz.");
    return true;
}

bool c_RadioManager::transmitPacket(const JoystickData& packet){
    int status = LoRa.beginPacket();

    if(status==9){
        LOG_ERROR("Radio buffer blocked. Cannot transmit packet.");
        return false;
    }

    LoRa.write((uint8_t*)&packet, sizeof(packet));

    if(LoRa.endPacket()) {
        return true;
    } else {
        LOG_ERROR("Radio transmission failed to clear.");
        return false;    
    }
}

bool c_RadioManager::recievePacket(TelemetryPacket& packet){
    int packetSize = LoRa.parsePacket();

    if(packetSize==0)return false;

    if(packetSize != sizeof(TelemetryPacket)){
        LOG_ERROR("Received packet with invalid size! Rejecting data.");
        return false;
    }

    uint8_t* packetPointer = (uint8_t*)&packet;
    for (size_t i = 0; i < sizeof(TelemetryPacket); i++) {
        if (LoRa.available()) {
            packetPointer[i] = LoRa.read();
        }
    }

    return true;
}
