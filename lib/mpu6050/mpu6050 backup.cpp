#include "mpu6050.h"         // MPU6050 Sensor header file 
#include "kalman.h"          // Kalman Filter header file
#include <iostream>

int file_i2c;
int length;
unsigned char buffer[60] = {0};

int16_t accel_offset_x = 336;
int16_t accel_offset_y = 188;
int16_t accel_offset_z = 689;
int16_t gyro_offset_x = -26;
int16_t gyro_offset_y = -24;
int16_t gyro_offset_z = -27;

float sampleFrequency = 100.0f; // Sample Frequency in Hz (try 100 or 200)

double gyro_noise = 0.01;        // convertt to rad
double accel_noise = 0.1; // convert to m/s^2

// Declare and initialize the control noise
double control_noise_roll = 0.01;
double control_noise_pitch = 0.01;

KalmanFilter kalmanRoll(0.001, 0.003, 0.03);
KalmanFilter kalmanPitch(0.001, 0.003, 0.03);

float accPitch = 0;
float accRoll = 0;

float kalPitch = 0;
float kalRoll = 0;

void read_raw_data(int addr, int16_t &high, int16_t &low) {
    buffer[0] = addr;
    length = 2;
    if (write(file_i2c, buffer, 1) != 1 || read(file_i2c, buffer, length) != length) {
        std::cout << "Failed to read from the sensor" << std::endl;
        exit(1);
    }
    high = buffer[0] << 8 | buffer[1];
    low = buffer[2] << 8 | buffer[3];
}

int main() {
    //----- OPEN THE I2C BUS -----
    char *filename = (char*)"/dev/i2c-2";
    if ((file_i2c = open(filename, O_RDWR)) < 0) {
        std::cout << "Failed to open the i2c bus" << std::endl;
        return 1;
    }

    if (ioctl(file_i2c, I2C_SLAVE, MPU6050_ADDR) < 0) {
        std::cout << "Failed to connect to the sensor" << std::endl;
        return 1;
    }

    //----- TURN ON THE MPU6050 -----
    buffer[0] = PWR_MGMT_1;
    buffer[1] = 0;
    length = 2;

    if (write(file_i2c, buffer, length) != length) {
        std::cout << "Failed to wake up the sensor" << std::endl;
        return 1;
    }

    //----- Loop for readings -----
    while (true) {
        //----- READ ACCELERATION DATA -----
        buffer[0] = ACCEL_XOUT_H;
        length = 6;
        if (write(file_i2c, buffer, 1) != 1 || read(file_i2c, buffer, length) != length) {
            std::cout << "Failed to read from the sensor" << std::endl;
            return 1;
        }

        int16_t accel_x = (buffer[0] << 8 | buffer[1]) - accel_offset_x;
        int16_t accel_y = (buffer[2] << 8 | buffer[3]) - accel_offset_y;
        int16_t accel_z = (buffer[4] << 8 | buffer[5]) - accel_offset_z;

        // Print raw acceleration values
        std::cout << "Raw Acceleration - X: " << accel_x << ", Y: " << accel_y << ", Z: " << accel_z << std::endl;


        // Convert raw data directly to m/s^2
        double ax_m_s2 = accel_x * (9.81 / 16384.0);
        double ay_m_s2 = accel_y * (9.81 / 16384.0);
        double az_m_s2 = accel_z * (9.81 / 16384.0);

        //----- READ GYROSCOPE DATA -----
        buffer[0] = GYRO_XOUT_H;
        length = 6;
        if (write(file_i2c, buffer, 1) != 1 || read(file_i2c, buffer, length) != length) {
            std::cout << "Failed to read from the sensor" << std::endl;
            return 1;
        }

        int16_t gyro_x = (buffer[0] << 8 | buffer[1]) - gyro_offset_x;
        int16_t gyro_y = (buffer[2] << 8 | buffer[3]) - gyro_offset_y;
        int16_t gyro_z = (buffer[4] << 8 | buffer[5]) - gyro_offset_z;

        // Print raw gyroscope values
        std::cout << "Raw Gyroscope - X: " << gyro_x << ", Y: " << gyro_y << ", Z: " << gyro_z << std::endl;

        // Convert raw data directly to rad/s
        double gx_deg_s = gyro_x * (1000.0 / 32768.0);
        double gy_deg_s = gyro_y * (1000.0 / 32768.0);
        double gz_deg_s = gyro_z * (1000.0 / 32768.0);

        // print converted values
        std::cout << "Gyro - X: " << gx_deg_s << ", Y: " << gy_deg_s << ", Z: " << gz_deg_s << std::endl;
        std::cout << "Accel - X: " << ax_m_s2 << ", Y: " << ay_m_s2 << ", Z: " << az_m_s2 << std::endl;

        // print no filtered values
        // std::cout << "Roll_nf: " << atan2(ay, az) * 180 / M_PI << ", Pitch_nf: " << atan2(-ax, sqrt(ay * ay + az * az)) * 180 / M_PI << std::endl;

        // // Predict the next state and covariance
        // kalmanFilterRoll.predict(0.0, control_noise_roll);
        // kalmanFilterPitch.predict(0.0, control_noise_pitch);

        // Update the state and covariance with the new measurements
        double pitch = atan2(az_m_s2, ax_m_s2); //pitch
        double roll = atan2(-ay_m_s2, sqrt(ax_m_s2*ax_m_s2 + az_m_s2*az_m_s2)); //roll

        // Convert Pith & Roll (deg)
        accPitch = pitch * (180.0 / M_PI);
        accRoll = roll * (180.0 / M_PI);

        //print psi and phi
        std::cout << "Pitch: " << accPitch << ", Roll: " << accRoll << std::endl;

        // Kalman Filter
        kalPitch = kalmanPitch.update(accPitch, gy_deg_s);
        kalRoll = kalmanRoll.update(accRoll, gz_deg_s);

        // // Print the filtered roll and pitch
        std::cout << "Roll: " << kalRoll << ", Pitch: " << kalPitch << std::endl;

        usleep(1000000 / sampleFrequency); // Sleep for 1/sampleFrequency seconds (200Hz = 5ms, 100Hz = 10ms, etc.
    }

    return 0;
}


