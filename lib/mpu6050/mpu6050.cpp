#include "mpu6050.h" 
#include <iostream>

// Constructor
MPU6050::MPU6050(float sampleFrequency) : sampleFrequency(sampleFrequency) {
        /*to use this:

        try {
            MPU6050 mpu(100.0f); // pass sample freq
        } catch (const std::runtime_error& e) {
            std::cerr << e.what() << std::endl;
            return 1;
        }
        
        */
    //----- OPEN THE I2C BUS -----
    char *filename = (char*)"/dev/i2c-2";
    if ((file_i2c = open(filename, O_RDWR)) < 0) {
        throw std::runtime_error("Failed to open the i2c bus");
    }

    if (ioctl(file_i2c, I2C_SLAVE, MPU6050_ADDR) < 0) {
        throw std::runtime_error("Failed to connect to the sensor");
    }

    //----- TURN ON THE MPU6050 -----
    buffer[0] = PWR_MGMT_1;
    buffer[1] = 0;
    length = 2;

    if (write(file_i2c, buffer, length) != length) {
        throw std::runtime_error("Failed to wake up the sensor");
    }
    // wait 1s and print that is ready
    usleep(1000000);
    std::cout << "MPU6050 is ready" << std::endl;
}

// Destructor
MPU6050::~MPU6050() {
    // Close the i2c bus
    if (file_i2c != -1) {
        close(file_i2c);
    }
}

MPU6050::FilteredData MPU6050::getFilteredData() {
    updateSensorData();
    FilteredData data;
    data.pitch = kalPitch;
    data.roll = kalRoll;
    return data;
}

void MPU6050::read_raw_data(int addr, int16_t &high, int16_t &low) {
    buffer[0] = addr;
    length = 2;
    if (write(file_i2c, buffer, 1) != 1 || read(file_i2c, buffer, length) != length) {
        std::cout << "Failed to read from the sensor" << std::endl;
        exit(1);
    }
    high = buffer[0] << 8 | buffer[1];
    low = buffer[2] << 8 | buffer[3];
}

void MPU6050::updateSensorData() {
    //------------- READ ACCELERATION DATA --------------
    buffer[0] = ACCEL_XOUT_H;
    length = 6;
    if (write(file_i2c, buffer, 1) != 1 || read(file_i2c, buffer, length) != length) {
        std::cout << "Failed to read from the sensor" << std::endl;
        return;
    }
    int16_t accel_x = (buffer[0] << 8 | buffer[1]) - accel_offset_x;
    int16_t accel_y = (buffer[2] << 8 | buffer[3]) - accel_offset_y;
    int16_t accel_z = (buffer[4] << 8 | buffer[5]) - accel_offset_z;
    // Convert raw data directly to m/s^2
    double ax_m_s2 = accel_x * (9.81 / 16384.0);
    double ay_m_s2 = accel_y * (9.81 / 16384.0);
    double az_m_s2 = accel_z * (9.81 / 16384.0);
    // ----------------------------------------
     //-------------- READ GYROSCOPE DATA -------------------
    buffer[0] = GYRO_XOUT_H;
    length = 6;
    if (write(file_i2c, buffer, 1) != 1 || read(file_i2c, buffer, length) != length) {
        std::cout << "Failed to read from the sensor" << std::endl;
        return;
    }
    int16_t gyro_x = (buffer[0] << 8 | buffer[1]) - gyro_offset_x;
    int16_t gyro_y = (buffer[2] << 8 | buffer[3]) - gyro_offset_y;
    int16_t gyro_z = (buffer[4] << 8 | buffer[5]) - gyro_offset_z;
     // Convert raw data directly to rad/s
    double gx_deg_s = gyro_x * (1000.0 / 32768.0);
    double gy_deg_s = gyro_y * (1000.0 / 32768.0);
    double gz_deg_s = gyro_z * (1000.0 / 32768.0);
    // get accPitch and accRoll
    double accPitch = atan2(az_m_s2, ax_m_s2) * 180/M_PI; //pitch
    double accRoll = atan2(-ay_m_s2, sqrt(ax_m_s2*ax_m_s2 + az_m_s2*az_m_s2)) * 180/M_PI; //roll
    // ----------------------------------------------------------
     // Kalman Filter
    kalPitch = kalmanPitch.update(accPitch, gy_deg_s);
    kalRoll = kalmanRoll.update(accRoll, gz_deg_s);
}

