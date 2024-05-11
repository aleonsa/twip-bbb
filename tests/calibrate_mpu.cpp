#include "../lib/mpu6050/mpu6050.h"         // MPU6050 Sensor header file 
#include "../lib/mpu6050/kalman.h"          // Kalman Filter header file
#include <iostream>
#include <vector>
#include <numeric>

int file_i2c;
int length;
unsigned char buffer[60] = {0};

int16_t accel_offset_x = 0;
int16_t accel_offset_y = 0;
int16_t accel_offset_z = 0;
int16_t gyro_offset_x = 0;
int16_t gyro_offset_y = 0;
int16_t gyro_offset_z = 0;

float sampleFrequency = 100.0f; // Sample Frequency in Hz (try 100 or 200)

double gyro_noise = 0.05*M_PI / 180;        // convertt to rad
double accel_noise = 9.81*400 / 1000000.0; // convert to m/s^2

// Declare and initialize the control noise
double control_noise_roll = 0.01;
double control_noise_pitch = 0.01;

KalmanFilter kalmanFilterRoll;  // Kalman filter for roll
KalmanFilter kalmanFilterPitch; // Kalman filter for pitch

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
    // Initialize the Kalman filters
    kalmanFilterRoll.init(0.0, 1.0);
    kalmanFilterPitch.init(0.0, 1.0);

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
    std::vector<int16_t> accel_x_samples, accel_y_samples, accel_z_samples;
    std::vector<int16_t> gyro_x_samples, gyro_y_samples, gyro_z_samples;

    for (int i = 0; i < 1000; ++i) { // Collect 1000 samples for calibration

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
        double ax_m_s2 = accel_x * (9.81 / 16384.0) - 16384.0;
        double ay_m_s2 = accel_y * (9.81 / 16384.0);
        double az_m_s2 = accel_z * (9.81 / 16384.0);

        accel_x_samples.push_back(buffer[0] << 8 | buffer[1]);
        accel_y_samples.push_back(buffer[2] << 8 | buffer[3]);
        accel_z_samples.push_back(buffer[4] << 8 | buffer[5]);


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

        // // print no filtered values
        // std::cout << "Roll_nf: " << atan2(ay, az) * 180 / M_PI << ", Pitch_nf: " << atan2(-ax, sqrt(ay * ay + az * az)) * 180 / M_PI << std::endl;

        // // Predict the next state and covariance
        // kalmanFilterRoll.predict(0.0, control_noise_roll);
        // kalmanFilterPitch.predict(0.0, control_noise_pitch);

        // // Update the state and covariance with the new measurements
        // double roll = atan2(ay, az);
        // double pitch = atan2(-ax, sqrt(ay * ay + az * az));
        // kalmanFilterRoll.update(roll, gyro_noise); // Use gyro_noise for roll
        // kalmanFilterPitch.update(pitch, accel_noise); // Use accel_noise for pitch

        // // Use the state of the Kalman filters as the filtered sensor readings
        // double roll_filtered = kalmanFilterRoll.getState();
        // double pitch_filtered = kalmanFilterPitch.getState();

        // // Print the filtered roll and pitch
        // std::cout << "Roll: " << roll_filtered * 180 / M_PI << ", Pitch: " << pitch_filtered * 180 / M_PI << std::endl;
        gyro_x_samples.push_back(buffer[0] << 8 | buffer[1]);
        gyro_y_samples.push_back(buffer[2] << 8 | buffer[3]);
        gyro_z_samples.push_back(buffer[4] << 8 | buffer[5]);


        usleep(1000000 / sampleFrequency); // Sleep for 1/sampleFrequency seconds (200Hz = 5ms, 100Hz = 10ms, etc.
    }

    // Calculate the mean of the samples to get the offsets
    accel_offset_x = std::accumulate(accel_x_samples.begin(), accel_x_samples.end(), 0) / accel_x_samples.size(); // Add gravity (1g)
    accel_offset_y = std::accumulate(accel_y_samples.begin(), accel_y_samples.end(), 0) / accel_y_samples.size();
    accel_offset_z = std::accumulate(accel_z_samples.begin(), accel_z_samples.end(), 0) / accel_z_samples.size(); 

    gyro_offset_x = -std::accumulate(gyro_x_samples.begin(), gyro_x_samples.end(), 0) / gyro_x_samples.size();
    gyro_offset_y = -std::accumulate(gyro_y_samples.begin(), gyro_y_samples.end(), 0) / gyro_y_samples.size();
    gyro_offset_z = -std::accumulate(gyro_z_samples.begin(), gyro_z_samples.end(), 0) / gyro_z_samples.size();

    std::cout << "Calibration complete. Offsets: " << std::endl;
    std::cout << "Accel - X: " << accel_offset_x << ", Y: " << accel_offset_y << ", Z: " << accel_offset_z << std::endl;
    std::cout << "Gyro - X: " << gyro_offset_x << ", Y: " << gyro_offset_y << ", Z: " << gyro_offset_z << std::endl;


    return 0;
}

// g++ -o mpu_test calibrate_mpu.cpp ../lib/mpu6050/kalman.cpp  && ./calibrate_mpu


/* **************calculated offsets***********************
int16_t accel_offset_x = 336;
int16_t accel_offset_y = 188;
int16_t accel_offset_z = 689;
int16_t gyro_offset_x = -26;
int16_t gyro_offset_y = -24;
int16_t gyro_offset_z = -27;
**********************************************************/