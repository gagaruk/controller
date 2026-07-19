#pragma once

#include <Wire.h>
#include "ControllerConfig.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

class c_screen
{
public:
    c_screen(TwoWire &i2c, uint refresInterval);
    void init();
    void update_peripheral_data(const PeripheralPacket &new_data);
    void update_telemetry_data(const TelemetryPacket &new_data);
    void display_peripheral();
    void display_telemetry();

private:
    void _print_peripheral_labels();
    void _print_telemetry_labels();
    TwoWire *_i2c;

    PeripheralPacket _previous_peripheral;
    TelemetryPacket _previous_telemetry;
    PeripheralPacket _current_peripheral;
    TelemetryPacket _current_telemetry;

    uint _last_t;
    uint _refreshInterval;

    Adafruit_SSD1306 _display;
};