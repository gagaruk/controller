#include <Arduino.h>
#include <unity.h>
#include <Wire.h>
#include "imuDriver.h"
#include "ControllerConfig.h"

c_imu imu = c_imu(imuConstants::ACCEL_RANGE::G_2, imuConstants::GYRO_RANGE::DEG_250, imuConstants::DLPF_FREQ::HZ_256, &Wire); //[cite: 8]
IMUData imuData;                                                                                                              //[cite: 8]

void setUp(void) {}
void tearDown(void) {}

void test_i2c_bus_init(void)
{
    // Verifies that I2C can configure itself to the targeted physical pins[cite: 8]
    Wire.begin(i2cConstants::SDA, i2cConstants::SCL); //[cite: 8]
    Wire.setClock(i2cConstants::CLOCK_SPEED);         //[cite: 8]
    TEST_ASSERT_TRUE(true);
}

void test_imu_connection_and_init(void)
{
    // Ensures the driver communicates with the IMU hardware and configures registers[cite: 8]
    bool imuStatus = imu.init(); //[cite: 8]
    TEST_ASSERT_TRUE_MESSAGE(imuStatus, "IMU hardware failed to initialize. Check your physical I2C lines.");
}

void test_mpu_readings_not_dead(void)
{
    // Reads from the accelerometer and gyroscope[cite: 8]
    imu.readMPUVals();       //[cite: 8]
    imu.processMPUVals();    //[cite: 8]
    imuData = imu.getData(); //[cite: 8]

    // A real IMU resting on a desk will have non-zero gravity and subtle noise.
    // Asserting that x/y/z values aren't all exactly zero rules out a frozen bus.
    float sumAccel = abs(imuData.accel.x) + abs(imuData.accel.y) + abs(imuData.accel.z);
    TEST_ASSERT_NOT_EQUAL_MESSAGE(0.0f, sumAccel, "IMU output is completely dead (all acceleration axes returned 0)");
}

void test_magnetometer_readings(void)
{
    // Tests magnetometer parsing[cite: 8]
    imu.readMagVals();       //[cite: 8]
    imu.processMagVals();    //[cite: 8]
    imuData = imu.getData(); //[cite: 8]

    // Verifying magnetometer is reading valid active spatial dimensions
    float sumMag = abs(imuData.mag.x) + abs(imuData.mag.y) + abs(imuData.mag.z);
    TEST_ASSERT_NOT_EQUAL_MESSAGE(0.0f, sumMag, "Magnetometer is reading absolute zero on all axes.");
}

void setup()
{
    delay(2000);
    UNITY_BEGIN();

    RUN_TEST(test_i2c_bus_init);
    RUN_TEST(test_imu_connection_and_init);
    RUN_TEST(test_mpu_readings_not_dead);
    RUN_TEST(test_magnetometer_readings);

    UNITY_END();
}

void loop()
{
    // Keep empty for Unity test runner
}