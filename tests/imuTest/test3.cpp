#include <Arduino.h>
#include <Wire.h>
#include "imuDriver.h"
#include "config.h"

c_imu imu = c_imu(imuConstants::ACCEL_RANGE::G_2, imuConstants::GYRO_RANGE::DEG_250, imuConstants::DLPF_FREQ::HZ_256, &Wire);
unsigned long lastLoopTime = 0;
imuConstants::IMUData_t imuData;

void printMPUData(const imuConstants::IMUData_t &data);
void printMagData(const imuConstants::IMUData_t &data);

void setup()
{
    Serial.begin(115200);
    Wire.begin(i2cConstants::SDA, i2cConstants::SCL);
    Wire.setClock(i2cConstants::CLOCK_SPEED);

    imu.init();
}

void loop()
{
    unsigned long currentTime = micros();

    if (currentTime - lastLoopTime >= loopConstants::FAST_LOOP_TIME_us)
    {
        lastLoopTime = currentTime;

        imu.readMPUVals();
        imu.processMPUVals();
        imuData = imu.getData();
        printMPUData(imuData);
    }

    if (currentTime - lastLoopTime >= loopConstants::SLOW_LOOP_TIME_us)
    {
        lastLoopTime = currentTime;
        imu.readMagVals();
        imu.processMagVals();
        imuData = imu.getData();
        printMagData(imuData);
    }
}

void printMPUData(const imuConstants::IMUData_t &data)
{
    Serial.print("Gyro: ");
    Serial.print("X: ");
    Serial.print(data.gyro.x);
    Serial.print(" ");
    Serial.print("Y: ");
    Serial.print(data.gyro.y);
    Serial.print(" ");
    Serial.print("Z: ");
    Serial.println(data.gyro.z);

    Serial.print("Accel: ");
    Serial.print("X: ");
    Serial.print(data.accel.x);
    Serial.print(" ");
    Serial.print("Y: ");
    Serial.print(data.accel.y);
    Serial.print(" ");
    Serial.print("Z: ");
    Serial.println(data.accel.z);
}

void printMagData(const imuConstants::IMUData_t &data)
{
    Serial.print("Mag: X=");
    Serial.print(data.mag.x);
    Serial.print(", Y=");
    Serial.print(data.mag.y);
    Serial.print(", Z=");
    Serial.println(data.mag.z);

    Serial.println("\n-------------------------------------\n");
}
