#include "imuDriver.h"

c_imu::c_imu(imuConstants::ACCEL_RANGE accelRange, imuConstants::GYRO_RANGE gyroRange, imuConstants::DLPF_FREQ dlpfFreq, TwoWire *i2c)
    : _accelRange(accelRange), _gyroRange(gyroRange), _dlpfFreq(dlpfFreq), _i2c(i2c)
{
}

bool c_imu::init()
{
    try
    {

        if (!_writeToDevice(imuConstants::MPU_ADDR, 0x6B, 0x01))
        {
            Serial.println("Error: Failed to wake up MPU6050!");
            return false;
        }
        delay(50);

        uint8_t gyroConfig = _translateGyroRange();
        if (!_writeToDevice(imuConstants::MPU_ADDR, 0x1B, gyroConfig))
            return false;

        uint8_t accelConfig = _translateAccelRange();
        if (!_writeToDevice(imuConstants::MPU_ADDR, 0x1C, accelConfig))
            return false;

        uint8_t dlpfConfig = _translateDlpfBits();
        if (!_writeToDevice(imuConstants::MPU_ADDR, imuConstants::CONFIG_ADDR, dlpfConfig))
            return false;

        return true;
    }
    catch (...)
    {
        return false;
    }
}

bool c_imu::readMPUVals()
{
    _i2c->beginTransmission(imuConstants::MPU_ADDR);
    _i2c->write(0x3B);

    if (_i2c->endTransmission(false) != 0)
    {
        return false;
    }

    uint8_t bytesReceived = _i2c->requestFrom((uint8_t)imuConstants::MPU_ADDR, (uint8_t)14);

    if (bytesReceived >= 14)
    {
        _data.accel.x = ((int16_t)_i2c->read() << 8) | ((int16_t)_i2c->read());
        _data.accel.y = ((int16_t)_i2c->read() << 8) | ((int16_t)_i2c->read());
        _data.accel.z = ((int16_t)_i2c->read() << 8) | ((int16_t)_i2c->read());

        (void)_i2c->read();
        (void)_i2c->read();

        _data.gyro.x = ((int16_t)_i2c->read() << 8) | ((int16_t)_i2c->read());
        _data.gyro.y = ((int16_t)_i2c->read() << 8) | ((int16_t)_i2c->read());
        _data.gyro.z = ((int16_t)_i2c->read() << 8) | ((int16_t)_i2c->read());

        return true;
    }

    return false;
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

void c_imu::getData(IMUData &data)
{
    data = _data;
}

bool c_imu::_writeToDevice(uint8_t deviceAddr, uint8_t reg, uint8_t val)
{
    _i2c->beginTransmission(deviceAddr);
    _i2c->write(reg);
    _i2c->write(val);
    _i2c->endTransmission();

    return (_i2c->endTransmission() == 0);
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
