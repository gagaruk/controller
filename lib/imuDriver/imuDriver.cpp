#include "imuDriver.h"

c_imu::c_imu(imuConstants::ACCEL_RANGE accelRange, imuConstants::GYRO_RANGE gyroRange, imuConstants::DLPF_FREQ dlpfFreq, TwoWire *i2c) : _accelRange(accelRange),
                                                                                                                                         _gyroRange(gyroRange),
                                                                                                                                         _dlpfFreq(dlpfFreq),
                                                                                                                                         _i2c(i2c)
{
}

bool c_imu::init()
{
    try
    {
        _writeToDevice(imuConstants::POWER_ADDR, imuConstants::POWER_ADDR, 0x00);
        delay(30);

        uint8_t gyroConfig = _translateGyroRange();
        _writeToDevice(imuConstants::MPU_ADDR, 0x1B, gyroConfig);

        uint8_t accelConfig = _translateAccelRange();
        _writeToDevice(imuConstants::MPU_ADDR, 0x1C, accelConfig);

        uint8_t dlpfConfig = _translateDlpfBits();
        _writeToDevice(imuConstants::MPU_ADDR, imuConstants::CONFIG_ADDR, dlpfConfig);

        _initMagnetometer();
        return true;
    }
    catch (...)
    {
        return false;
    }
}
void c_imu::readMPUVals()
{
    _i2c->beginTransmission(imuConstants::MPU_ADDR);
    _i2c->write(0x3B);
    _i2c->endTransmission(false);
    _i2c->requestFrom(imuConstants::MPU_ADDR, 14);

    if (_i2c->available() >= 14)
    {
        _data.accel.x = ((int16_t)_i2c->read() << 8) | ((int16_t)_i2c->read());
        _data.accel.y = ((int16_t)_i2c->read() << 8) | ((int16_t)_i2c->read());
        _data.accel.z = ((int16_t)_i2c->read() << 8) | ((int16_t)_i2c->read());

        (void)_i2c->read();
        (void)_i2c->read();

        _data.gyro.x = ((int16_t)_i2c->read() << 8) | ((int16_t)_i2c->read());
        _data.gyro.y = ((int16_t)_i2c->read() << 8) | ((int16_t)_i2c->read());
        _data.gyro.z = ((int16_t)_i2c->read() << 8) | ((int16_t)_i2c->read());
    }
}

void c_imu::processMPUVals()
{
    _data.gyro.x /= _gyroScaleDivider;
    _data.gyro.y /= _gyroScaleDivider;
    _data.gyro.z /= _gyroScaleDivider;

    _data.accel.x /= _accelScaleDivider;
    _data.accel.y /= _accelScaleDivider;
    _data.accel.z /= _accelScaleDivider;
}

void c_imu::readMagVals()
{
    _writeToDevice(imuConstants::MAG_ADDR, 0x0A, 0x01);

    _i2c->beginTransmission(imuConstants::MAG_ADDR);
    _i2c->write(0x03);
    _i2c->endTransmission(false);

    _i2c->requestFrom(imuConstants::MAG_ADDR, 6);
    if (_i2c->available() >= 6)
    {
        _data.mag.x = (int16_t)(((uint16_t)_i2c->read()) | ((uint16_t)_i2c->read() << 8));
        _data.mag.z = (int16_t)(((uint16_t)_i2c->read()) | ((uint16_t)_i2c->read() << 8));
        _data.mag.y = (int16_t)(((uint16_t)_i2c->read()) | ((uint16_t)_i2c->read() << 8));
    }
}
void c_imu::processMagVals()
{
    _data.mag.x *= _magScaleDividerX;
    _data.mag.y *= _magScaleDividerY;
    _data.mag.z *= _magScaleDividerZ;
}

IMUData c_imu::getData()
{
    IMUData latestData = _data;
    return latestData;
}

void c_imu::_writeToDevice(uint8_t deviceAddr, uint8_t reg, uint8_t val)
{
    _i2c->beginTransmission(deviceAddr);
    _i2c->write(reg);
    _i2c->write(val);
    _i2c->endTransmission();
}
uint8_t c_imu::_translateGyroRange()
{
    switch (_gyroRange)
    {
    case imuConstants::GYRO_RANGE::DEG_250:
        _gyroScaleDivider = 131.0f;
        return 0x00;
    case imuConstants::GYRO_RANGE::DEG_500:
        _gyroScaleDivider = 65.5f;
        return 0x08;
    case imuConstants::GYRO_RANGE::DEG_1000:
        _gyroScaleDivider = 32.8f;
        return 0x10;
    case imuConstants::GYRO_RANGE::DEG_2000:
        _gyroScaleDivider = 16.4f;
        return 0x08;
    default:
        return 0x00;
    }
}
uint8_t c_imu::_translateAccelRange()
{
    switch (_accelRange)
    {
    case imuConstants::ACCEL_RANGE::G_2:
        _accelScaleDivider = 16384.0f;
        return 0x00;
    case imuConstants::ACCEL_RANGE::G_4:
        _accelScaleDivider = 8192.0f;
        return 0x08;
    case imuConstants::ACCEL_RANGE::G_8:
        _accelScaleDivider = 4096.0f;
        return 0x10;
    case imuConstants::ACCEL_RANGE::G_16:
        _accelScaleDivider = 2048.0f;
        return 0x18;
    default:
        return 0x00;
    }
}

uint8_t c_imu::_translateDlpfBits()
{
    switch (_dlpfFreq)
    {
    case imuConstants::DLPF_FREQ::HZ_256:
        return 0x00;
    case imuConstants::DLPF_FREQ::HZ_188:
        return 0x01;
    case imuConstants::DLPF_FREQ::HZ_98:
        return 0x02;
    case imuConstants::DLPF_FREQ::HZ_42:
        return 0x03;
    case imuConstants::DLPF_FREQ::HZ_20:
        return 0x04;
    case imuConstants::DLPF_FREQ::HZ_10:
        return 0x05;
    case imuConstants::DLPF_FREQ::HZ_5:
        return 0x06;
    }
    return 0x00;
}

void c_imu::_initMagnetometer()
{

    _writeToDevice(imuConstants::MAG_ADDR, 0x37, 0x02);
    delay(10);

    _writeToDevice(imuConstants::MAG_ADDR, 0x0A, 0x0F);
    delay(20);

    Wire.beginTransmission(imuConstants::MAG_ADDR);
    Wire.write(0x10);
    Wire.endTransmission(false);

    Wire.requestFrom(imuConstants::MAG_ADDR, 3);
    if (Wire.available() >= 3)
    {
        uint8_t asax = Wire.read();
        uint8_t asay = Wire.read();
        uint8_t asaz = Wire.read();

        _magScaleDividerX = 0.3f * (((float)(asax - 128) / 256.0f) + 1.0f);
        _magScaleDividerY = 0.3f * (((float)(asay - 128) / 256.0f) + 1.0f);
        _magScaleDividerZ = 0.3f * (((float)(asaz - 128) / 256.0f) + 1.0f);
    }
    _writeToDevice(imuConstants::MAG_ADDR, 0x0A, 0x00);
    delay(10);

    _writeToDevice(imuConstants::MAG_ADDR, 0x0A, 0x01);
    delay(20);
}
