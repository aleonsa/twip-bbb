#ifndef MPU6050_H
#define MPU6050_H

#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <cmath>
#include "kalman.h" // Use quotes for local includes

class MPU6050 {
public:
    struct FilteredData {
        float pitch;
        float roll;
    };

    static const int MPU6050_ADDR = 0x68;
    static const int PWR_MGMT_1 = 0x6B;
    static const int ACCEL_XOUT_H = 0x3B;
    static const int ACCEL_XOUT_L = 0x3C;
    static const int ACCEL_YOUT_H = 0x3D;
    static const int ACCEL_YOUT_L = 0x3E;
    static const int ACCEL_ZOUT_H = 0x3F;
    static const int ACCEL_ZOUT_L = 0x40;
    static const int GYRO_XOUT_H = 0x43;
    static const int GYRO_XOUT_L = 0x44;
    static const int GYRO_YOUT_H = 0x45;
    static const int GYRO_YOUT_L = 0x46;
    static const int GYRO_ZOUT_H = 0x47;
    static const int GYRO_ZOUT_L = 0x48;
    static const int GYRO_CONFIG = 0x1B;

    MPU6050(float sampleFrequency);
    ~MPU6050();
    float getPitch();
    float getRoll();
    FilteredData getFilteredData();

private:
    void read_raw_data(int addr, int16_t &high, int16_t &low);
    void updateSensorData();

    int file_i2c;
    int length;
    unsigned char buffer[60] = {0};

    int16_t accel_offset_x = 336;
    int16_t accel_offset_y = 188;
    int16_t accel_offset_z = 689;
    int16_t gyro_offset_x = -26;
    int16_t gyro_offset_y = -24;
    int16_t gyro_offset_z = -27;

    float sampleFrequency; // Removed initialization here

    KalmanFilter kalmanRoll;
    KalmanFilter kalmanPitch;

    float accPitch = 0;
    float accRoll = 0;

    float kalPitch = 0;
    float kalRoll = 0;
};

#endif  // MPU6050_H