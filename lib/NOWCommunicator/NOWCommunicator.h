#pragma once 

#include <esp_now.h>
#include <WiFi.h>

class c_NOWCommunictor{
    c_NOWCommunictor();
    void getMacAddress(uint8_t* macAddress_ptr);
    bool init();
    bool transferData();
    bool recieveData();
};