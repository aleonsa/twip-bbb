#include "../lib/mpu6050/mpu6050.h"
#include "../src/Controllers/pid.h"
#include <iostream>
#include <fstream>
#include <chrono>

int main()
{
    // print "running..."
    std::cout << "running..." << std::endl;
    // upright angle correction
    float upright_angle = 0.0f;
    try
    {
        float sampleFrequency = 200.0f; // Sample frequency
        MPU6050 mpu(sampleFrequency);   // Create an MPU6050 object with a sample frequency

        std::ofstream file("pid_test.csv");                         // Create an output file stream
        file << "Time,Pitch,Error,dError,IntError,ControlSignal\n"; // Write the header to the file

        auto start = std::chrono::high_resolution_clock::now(); // Get the start time

        // Create a PID controller with specific Kp, Ki, Kd values
        PID pidController(7.0, 0.001, 0.3);

        auto previousTime = std::chrono::high_resolution_clock::now(); // Get the previous time before the loop starts
        
        while (true)
        {                                                       // Loop indefinitely
            MPU6050::FilteredData data = mpu.getFilteredData(); // Get the filtered data

            auto now = std::chrono::high_resolution_clock::now();                               // Get the current time
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - start); // Calculate the elapsed time

            // calculate error
            double error = data.pitch - upright_angle;

            // calculate real dt
            auto dt = std::chrono::duration<double>(now - previousTime).count();
            previousTime = now; // update previousTime for the next loop iteration

            // calculate control signal using PID controller
            double controlSignal = pidController.calculate(error, dt);

            // get integral and derivative of the error
            double integralError = pidController.getIntegral();
            double derivativeError = pidController.getDerivative();

            // Write the time, pitch, error, controlSignal, integralError, and derivativeError to the file
            file << duration.count() << "," << data.pitch << "," << error << "," << derivativeError << "," << integralError << "," << controlSignal << "\n";

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

// g++ -o pid_test pid_test.cpp ../lib/mpu6050/mpu6050.cpp ../lib/mpu6050/kalman.cpp ../src/Controllers/pid.cpp && ./pid_test