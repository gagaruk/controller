#include "OledDisplay.h"

c_screen::c_screen(const TwoWire &i2c, uint refreshInterval) : _i2c(i2c), _refreshInterval(refreshInterval)
{

    Adafruit_SSD1306 display(displayConstants::SCREEN_WIDTH, displayConstants::SCREEN_HEIGHT, &_i2c, -1);
    _display = display;
}

void c_screen::init()
{
    _display.clearDisplay();
    _display.setTextSize(1);
    _display.setTextColor(SSD1306_WHITE);

    _print_peripheral_labels();
}

void c_screen::update_peripheral_data(const PeripheralPacket &new_data)
{
    _previous_peripheral = _current_peripheral;
    _current_peripheral = new_data;
}
void c_screen::update_telemetry_data(const TelemetryPacket &new_data)
{
    _previous_telemetry = _current_telemetry;
    _current_telemetry = new_data;
}

void c_screen::_print_peripheral_labels()
{
    //_display.setCursor(0, 0);
    //_display.print("TX: ");

    _display.setCursor(0, 11);
    _display.print("P:");
    _display.setCursor(64, 11);
    _display.print("R:");
    _display.setCursor(0, 20);
    _display.print("Y:");

    _display.setCursor(0, 31);
    _display.print("BTNS: ");

    _display.setCursor(0, 44);
    _display.print("J1:");
    _display.setCursor(0, 54);
    _display.print("J2:");
    _display.setCursor(90, 54);
    _display.print("TG:");

    _display.display();
}
void c_screen::display_peripheral()
{
    if ((millis() - _last_t) < _refreshInterval)
        return;
    _display.setTextColor(SSD1306_WHITE, SSD1306_BLACK);

    /* if (strcmp(current.tx_mode, lastDisplayedData.tx_mode) != 0) {
         display.setCursor(24, 0);
         display.print(current.tx_mode);

         display.print("    ");
         needsUpdate = true;
     }
     */
    // This logic hasn't been programmed into the rest of the code since most of the projects will be using a single kind of transmission

    if (abs(_current_peripheral.imuData.gyro.x - _previous_peripheral.imuData.gyro.x) > 0.1 ||
        abs(_current_peripheral.imuData.gyro.y - _previous_peripheral.imuData.gyro.y) > 0.1 ||
        abs(_current_peripheral.imuData.gyro.z - _previous_peripheral.imuData.gyro.x) > 0.1)
    {

        _display.setCursor(14, 11);
        _display.printf("%5.1f ", _current_peripheral.imuData.gyro.x);
        _display.setCursor(78, 11);
        _display.printf("%5.1f ", _current_peripheral.imuData.gyro.y);
        _display.setCursor(14, 20);
        _display.printf("%5.1f ", _current_peripheral.imuData.gyro.z);
    }

    _display.setCursor(32, 31);
    if (_current_peripheral.buttArr.w != _previous_peripheral.buttArr.w)
    {
        _display.print(_current_peripheral.buttArr.w);
    }
    else
    {
        _display.setCursor(_display.getCursorX() + 6, _display.getCursorY());
    }
    if (_current_peripheral.buttArr.a != _previous_peripheral.buttArr.a)
    {
        _display.print(_current_peripheral.buttArr.a);
    }
    else
    {
        _display.setCursor(_display.getCursorX() + 6, _display.getCursorY());
    }
    if (_current_peripheral.buttArr.s != _previous_peripheral.buttArr.s)
    {
        _display.print(_current_peripheral.buttArr.s);
    }
    else
    {
        _display.setCursor(_display.getCursorX() + 6, _display.getCursorY());
    }
    if (_current_peripheral.buttArr.d != _previous_peripheral.buttArr.d)
    {
        _display.print(_current_peripheral.buttArr.d);
    }
    else
    {
        _display.setCursor(_display.getCursorX() + 6, _display.getCursorY());
    }
    if (_current_peripheral.buttArr.x != _previous_peripheral.buttArr.x)
    {
        _display.print(_current_peripheral.buttArr.x);
    }
    else
    {
        _display.setCursor(_display.getCursorX() + 6, _display.getCursorY());
    }

    if (_current_peripheral.joy1.x != _previous_peripheral.joy1.x ||
        _current_peripheral.joy1.y != _previous_peripheral.joy1.y ||
        _current_peripheral.joy1.sw != _previous_peripheral.joy1.sw)
    {

        _display.setCursor(24, 44);
        _display.printf("X:%4d Y:%4d (%s)", _current_peripheral.joy1.x, _current_peripheral.joy1.y, _current_peripheral.joy1.sw ? "V" : " ");
    }

    if (_current_peripheral.joy2.x != _previous_peripheral.joy2.x ||
        _current_peripheral.joy2.y != _previous_peripheral.joy2.y ||
        _current_peripheral.joy2.sw != _previous_peripheral.joy2.sw)
    {

        _display.setCursor(24, 54);
        _display.printf("X:%4d Y:%4d (%s)", _current_peripheral.joy2.x, _current_peripheral.joy2.y, _current_peripheral.joy2.sw ? "V" : " ");
    }

    if (_current_peripheral.switchState != _previous_peripheral.switchState)
    {
        _display.setCursor(110, 54);
        _display.print(_current_peripheral.switchState ? "ON " : "OFF");
    }
}

void c_screen::_print_telemetry_labels() {}
void c_screen::display_telemetry() {}