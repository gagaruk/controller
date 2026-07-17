#pragma once 

#include <WiFi.h>
#include <esp_now.h>
#include <esp_wifi.h>

class c_EspNowManager{
public:
    c_EspNowManager();
    String getMacAddress();
    bool init(uint8_t wifiChannel, const uint8_t* targetMac);
    bool sendData(const uint8_t* targetMac, const uint8_t* data, size_t length);
    bool recievedData(TelemetryPacket& packet);
private:
    bool _registerPeer(const uint8_t* targetMac);
    static void _onDataSent(const uint8_t* mac_addr, esp_now_send_status_t status);
    static void _onDataRecv(const uint8_t* mac_addr, const uint8_t* incomingData, int len);
    static bool _newDataAvailable;
    static TelemetryPacket _latestPacket;
};