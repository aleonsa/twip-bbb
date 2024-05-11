#include "../lib/mpu6050/mpu6050.h"
#include <iostream>
#include <fstream>
#include <chrono>

int main() {
    try {
        float sampleFrequency = 200.0f; // Sample frequency
        MPU6050 mpu(sampleFrequency); // Create an MPU6050 object with a sample frequency of 100 Hz

        std::ofstream file("output.csv"); // Create an output file stream
        file << "Time,Pitch,Roll\n"; // Write the header to the file

        auto start = std::chrono::high_resolution_clock::now(); // Get the start time

        while (true) { // Loop indefinitely
            MPU6050::FilteredData data = mpu.getFilteredData(); // Get the filtered data

            auto now = std::chrono::high_resolution_clock::now(); // Get the current time
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - start); // Calculate the elapsed time

            file << duration.count() << "," << data.pitch << "," << data.roll << "\n"; // Write the time, pitch, and roll to the file

            usleep(1000000 / sampleFrequency); // Sleep for 1/sampleFrequency seconds (200Hz = 5ms, 100Hz = 10ms, etc.)
        }
    } catch (const std::runtime_error& e) {
        std::cerr << e.what() << std::endl; // Print the error message if an exception is thrown
        return 1;
    }
    return 0;
}

//g++ -o mpu-kalman_test mpu-kalman_test.cpp ../lib/mpu6050/mpu6050.cpp ../lib/mpu6050/kalman.cpp && ./mpu-kalman_test