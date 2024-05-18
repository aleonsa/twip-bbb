#include "../lib/mpu6050/mpu6050.h"
// #include "../src/Controllers/pid.h"
#include "../src/Observers/hgo.h"
//#include "../src/Controllers/pidlike.h"
// #include "../lib/motors/pwm.h"
// #include "../lib/motors/direction.h"
#include <unistd.h>
#include <thread>
#include <iostream>
#include <fstream>
#include <chrono>

int main()
{
    // print "running..."
    std::cout << "running main..." << std::endl;
    // upright angle correction
    float upright_angle = -1.824613f;
    // std::cout << "Enter upright angle correction: ";
    // std::cin >> upright_angle;
    float sampleFrequency = 1000.0f; // Sample frequency
    int sampleCount = 0; // count samples to wait the kalman filter to converge

    std::ofstream file("hgo_test_out.csv");                         // Create an output file stream
    file << "Time,Pitch,Error,y1,dt\n"; // Write the header to the file

    // ******************************** OBSERVER INITIALIZATION ****************************************
    HighGainObserver hgo;

    float L = 15; // 15 funciona a sampleTime = 1000;
    std::cout << "Enter L: ";
    std::cin >> L;

    auto start = std::chrono::high_resolution_clock::now(); // Get the start time

    std::cout << "Starting IMU..." << std::endl;
    std::cout << "Hold the robot upright..." << std::endl;
    usleep(5000 * 1000); // sleep for 5000 milliseconds (5s)
    try
    {
        MPU6050 mpu(sampleFrequency);   // Create an MPU6050 object with a sample frequency
        usleep(1000 * 1000); // sleep for 1000 milliseconds (1s)
        std::cout << "IMU OK." << std::endl;
        auto previousTime = std::chrono::high_resolution_clock::now(); // Get the previous time before the loop starts
        
        while (true)
        {                                                       // Loop indefinitely
            // Check the sample count, at 100 samples the kalman filter converges
            sampleCount++;
            if(sampleCount >= 101){
                sampleCount = 101;
            }
            // Get the filtered data
            MPU6050::FilteredData data = mpu.getFilteredData(); 

            // Get variables for dt
            auto now = std::chrono::high_resolution_clock::now();                               // Get the current time
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - start); // Calculate the elapsed time

            // calculate error
            double error = data.pitch - upright_angle;

            // calculate real dt
            auto dt = std::chrono::duration<double>(now - previousTime).count();
            previousTime = now; // update previousTime for the next loop iteration

            //calculate derivative of error
            auto [y1, y2] = hgo.hgo(error, L, dt);

            // Write the time, pitch, error, controlSignal, integralError, and derivativeError to the file
            file << duration.count() << "," << data.pitch << "," << error << "," << y1 << "," << dt << "\n";

            //usleep(1000000 / sampleFrequency); // Sleep for 1/sampleFrequency seconds (200Hz = 5ms, 100Hz = 10ms, etc.)
            
        }
    }
    catch (const std::runtime_error &e)
    {
        std::cerr << e.what() << std::endl; // Print the error message if an exception is thrown
        return 1;
    }
    return 0;
}

/* command to compile:
// high gain observer
g++ -o hgo_test hgo_test.cpp /home/debian/dev/twip-bbb/lib/mpu6050/mpu6050.cpp /home/debian/dev/twip-bbb/lib/mpu6050/kalman.cpp /home/debian/dev/twip-bbb/src/Observers/hgo.cpp -lgpiodcxx && ./hgo_test

*/