#include "../lib/mpu6050/mpu6050.h"
#include <iostream>
#include <chrono>
#include <unistd.h>
#include <fstream>

int main()
{
    std::cout << "Running upright angle correction program..." << std::endl;

    float sampleFrequency = 200.0f; // Sample frequency

    std::ofstream file("upright_angle_correction.csv"); // Create an output file stream
    file << "Sample,Pitch\n";                           // Write the header to the file

    try
    {
        MPU6050 mpu(sampleFrequency); // Create an MPU6050 object with a sample frequency
        usleep(1000 * 1000);          // sleep for 1000 milliseconds (1s)
        std::cout << "IMU OK." << std::endl;

        for (int i = 0; i < 1000; i++)
        {
            // Get the filtered data
            MPU6050::FilteredData data = mpu.getFilteredData();

            // Write the sample number and pitch data to the file
            file << i << "," << data.pitch << "\n";

            usleep(1000000 / sampleFrequency); // Sleep for 1/sampleFrequency seconds (200Hz = 5ms, 100Hz = 10ms, etc.)
        }
    }
    catch (const std::runtime_error &e)
    {
        std::cerr << e.what() << std::endl; // Print the error message if an exception is thrown
        return 1;
    }

    return 0;
}

// g++ -o calibrate_mpu calibrate_mpu.cpp /home/debian/dev/twip-bbb/lib/mpu6050/mpu6050.cpp /home/debian/dev/twip-bbb/lib/mpu6050/kalman.cpp && ./calibrate_mpu

// upright angle correction computed with matlab: -1.824613