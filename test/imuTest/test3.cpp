#include <Arduino.h>
#include <unity.h>
#include <Wire.h>
#include "imuDriver.h"
#include "ControllerConfig.h"

c_imu imu = c_imu(imuConstants::ACCEL_RANGE::G_2, imuConstants::GYRO_RANGE::DEG_250, imuConstants::DLPF_FREQ::HZ_256, &Wire);
IMUData imuData;

void print_imuData(const IMUData &data);

void setup()
{
    Serial.begin(115200);
    while (!Serial)
        ;
    delay(1000);

    Serial.println("Waking up I2C Wire Bus...");
    Wire.begin(i2cConstants::SDA, i2cConstants::SCL, i2cConstants::CLOCK_SPEED);
    Wire.setTimeOut(100);

    Serial.println("Attempting to initialize IMU chip...");

    Serial.println("--- BYPASSING DRIVER: DIRECT HARDWARE TEST ---");

    // 1. Force the physical chip to wake up directly
    Wire.beginTransmission(0x68);
    Wire.write(0x6B); // PWR_MGMT_1 register
    Wire.write(0x00); // 0x00 = Awake, internal 8MHz oscillator
    Wire.endTransmission();

    delay(100); // Wait for the MEMS silicon to stabilize

    // 2. Request exactly two raw bytes for the Z-Axis Accelerometer (Registers 0x3F and 0x40)
    Wire.beginTransmission(0x68);
    Wire.write(0x3F);
    Wire.endTransmission(false);

    // 3. Pull the bytes and construct the 16-bit integer
    Wire.requestFrom((uint16_t)0x68, (uint8_t)2);

    if (Wire.available() >= 2)
    {
        int16_t rawZ = (Wire.read() << 8) | Wire.read();
        Serial.printf("ABSOLUTE RAW Z-AXIS BITS: %d\n", rawZ);
    }
    else
    {
        Serial.println("I2C ERROR: Chip did not return 2 bytes.");
    }

    Serial.println("----------------------------------------------");

    bool success = imu.init();

    Serial.print("IMU Initialization Result: ");
    Serial.println(success ? "SUCCESS" : "FAILED");
}
void loop()
{
    if (imu.readMPUVals())
    {
        imu.processMPUVals();

        imu.getData(imuData);

        print_imuData(imuData);
    }
    else
    {
        Serial.println("Skipping frame: Bus was busy or packet was incomplete.");
    }

    delay(100);
}
void print_imuData(const IMUData &data)
{
    Serial.println("\n=================== IMU RAW METRICS ===================");

    Serial.printf("ACCEL => X: %7.2f | Y: %7.2f | Z: %7.2f\n",
                  data.accel.x, data.accel.y, data.accel.z);

    Serial.printf("GYRO  => X: %7.2f | Y: %7.2f | Z: %7.2f\n",
                  data.gyro.x, data.gyro.y, data.gyro.z);

    Serial.println("========================================================");
}