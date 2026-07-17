#pragma once

#include <Arduino.h>
#include <Wire.h>
#include "ControllerConfig.h"

class c_imu
{
private:
    TwoWire *_i2c;
    IMUData _data;

    imuConstants::ACCEL_RANGE _accelRange;
    imuConstants::GYRO_RANGE _gyroRange;
    imuConstants::DLPF_FREQ _dlpfFreq;

    float _accelScaleDivider;
    float _gyroScaleDivider;
    float _magScaleDividerX, _magScaleDividerY, _magScaleDividerZ;

    uint8_t _translateGyroRange();
    uint8_t _translateAccelRange();
    uint8_t _translateDlpfBits();
    void _writeToDevice(uint8_t deviceAddr, uint8_t reg, uint8_t val);
    void _initMagnetometer();

public:
    c_imu(imuConstants::ACCEL_RANGE accelRange, imuConstants::GYRO_RANGE gyroRange, imuConstants::DLPF_FREQ dlpfFreq, TwoWire *i2c);
    void init();
    void readMPUVals();
    void processMPUVals();
    void readMagVals();
    void processMagVals();
    IMUData getData();
};