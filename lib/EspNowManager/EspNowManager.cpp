#include "EspNowManager.h"

c_EspNowManager::c_EspNowManager() {}

bool c_EspNowManager::init(uint8_t wifiChannel, const uint8_t *targetMac)
{
    WiFi.mode(WIFI_STA);

    esp_wifi_set_channel(wifiChannel, WIFI_SECOND_CHAN_NONE);
    esp_wifi_set_protocol(EspNowConstants::WIFI_INTERFACE, EspNowConstants::WIFI_PROTOCOL);

    WiFi.setTxPower(EspNowConstants::TX_POWER);

    if (esp_now_init() != ESP_OK)
    {
        return false;
    }

    esp_now_register_send_cb(c_EspNowManager::_onDataSent);
    esp_now_register_recv_cb(c_EspNowManager::_onDataRecv);

    while (!_registerPeer(targetMac))
        ;

    return true;
}

bool c_EspNowManager::_registerPeer(const uint8_t *macAddress)
{
    if (esp_now_is_peer_exist(macAddress))
    {
        return true;
    }

    LOG_ERROR("Peer not found, adding peer...");
    esp_now_peer_info_t peerInfo = {};
    memcpy(peerInfo.peer_addr, macAddress, 6);
    peerInfo.channel = 0;
    peerInfo.encrypt = false;

    return (esp_now_add_peer(&peerInfo) == ESP_OK);
}

bool c_EspNowManager::sendData(const uint8_t *targetMac, const uint8_t *data, size_t length)
{
    if (length > 250)
    {
        return false;
    }

    if (!_registerPeer(targetMac))
    {
        return false;
    }

    esp_err_t result = esp_now_send(targetMac, data, length);
    return (result == ESP_OK);
}

void c_EspNowManager::_onDataSent(const uint8_t *mac_addr, esp_now_send_status_t status)
{
    if (status == ESP_NOW_SEND_SUCCESS)
    {
        LOG_INFO("ESP Now data succesfully sent");
    }
    else
    {
        LOG_ERROR("Esp Now data couldn't be recieved");
    }
}

void c_EspNowManager::_onDataRecv(const uint8_t *mac_addr, const uint8_t *incomingData, int len)
{
    if (len != sizeof(TelemetryPacket))
    {
        return;
    }
    memcpy(&_latestPacket, incomingData, len);
    _newDataAvailable = true;
}

bool c_EspNowManager::recievedData(TelemetryPacket &packet)
{
    portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;
    portENTER_CRITICAL(&timerMux);

    if (!_newDataAvailable)
    {
        portEXIT_CRITICAL(&timerMux);
        return false;
    }

    packet = _latestPacket;
    _newDataAvailable = false;

    portEXIT_CRITICAL(&timerMux);
    return true;
}

String c_EspNowManager::getMacAddress()
{
    WiFi.mode(WIFI_MODE_STA);
    return WiFi.macAddress();
}