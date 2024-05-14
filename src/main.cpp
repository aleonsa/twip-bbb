#include "../lib/mpu6050/mpu6050.h"
#include "../src/Controllers/pid.h"
//#include "../src/Controllers/pidlike.h"
#include "../lib/motors/pwm.h"
#include "../lib/motors/direction.h"
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

    // MOTOR'S DIRECTION INIT
    std::cout << "Initializing Motor's direction pins" << std::endl;
    Motor motor1("gpiochip2", 24, "gpiochip1", 29);
    Motor motor2("gpiochip1", 1, "gpiochip1", 5);

    // PWM INITIALIZE
    std::cout << "Initializing PWMs..." << std::endl;
    PWM pwmA("1", "a", "P9_14"); 
    PWM pwmB("1", "b", "P9_16");

    std::cout << "Setting PWM periods..." << std::endl;
    pwmA.setPeriod(1000000); // 1 second period
    pwmB.setPeriod(1000000); // 1 second period

    std::cout << "Enabling PWMs..." << std::endl;
    pwmA.enable();
    pwmB.enable();

    std::cout << "Starting PWM duty cycle adjustments..." << std::endl;
    float dutyCycle = 0.0;
    float min = -99.0;
    float max = 99.0;


    // ********************************* CONTROLLERS INITIALIZATION *************************************
        // Initialize PID instance and output file
    std::cout << "Initializing Controller..." << std::endl;
    std::ofstream file("main_out.csv");                         // Create an output file stream
    file << "Time,Pitch,Error,dError,IntError,ControlSignal\n"; // Write the header to the file

    auto start = std::chrono::high_resolution_clock::now(); // Get the start time

    // 1. ----------------------------- PID CONTROLLER ------------------------------------------
    // ... Create a PID controller with specific Kp, Ki, Kd values

    float Kp = 0.0; //12.0;
    float Ki = 0.0; //0.1;
    float Kd = 0.0; //0.05;
    std::cout << "Enter Kp: ";
    std::cin >> Kp;
    std::cout << "Enter Ki: ";
    std::cin >> Ki;
    std::cout << "Enter Kd: ";
    std::cin >> Kd;

    PID pidController(Kp, Ki, Kd);
    // -----------------------------------------------------------------------------------------
    // 2. ----------------------------- PID-LIKE (SMC) -----------------------------------------

    // float L = 0.0; // bound of the perturbation
    // std::cout << "Enter L: ";
    // std::cin >> L;

    // PIDLike pidlike(L);
    // -----------------------------------------------------------------------------------------

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

            // calculate control signal using PID controller
            double controlSignal = pidController.calculate(error, dt);
            // calculate control signal using PID-Like 
            //double controlSignal = pidlike.calculate(error,dt);

            // saturate the controlSignal
            if(controlSignal >= max){
                controlSignal = max;
            }else if(controlSignal <= min){
                controlSignal = min;
            }else{}//do nothing

            // if error is too big, turn off the control
            // if it is too small, might be noise
            if(abs(error) >= 17.0 || sampleCount<=100){
                controlSignal = 0.0;
            } 

            // decide the direction of the motors
            if (controlSignal < 0) {
                motor1.backward();
                motor2.backward();
                // std::cout << "forward" << std::endl;
            } else if (controlSignal >= 0) {
                motor1.forward();
                motor2.forward();
                // std::cout << "backward" << std::endl;
            } else {} // do nothing
            
            // Set the duty cycle based on ControlSignal
            dutyCycle = abs(controlSignal)/100;
            pwmA.setDutyCycle(dutyCycle);
            pwmB.setDutyCycle(dutyCycle);

            // get integral and derivative of the error
            double integralError = pidController.getIntegral();
            double derivativeError = pidController.getDerivative();
            // double integralError = 0;
            // double derivativeError = 0;

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

/* command to compile:
// PID
g++ -o main main.cpp /home/debian/dev/twip-bbb/lib/mpu6050/mpu6050.cpp /home/debian/dev/twip-bbb/lib/mpu6050/kalman.cpp /home/debian/dev/twip-bbb/src/Controllers/pid.cpp /home/debian/dev/twip-bbb/lib/motors/pwm.cpp /home/debian/dev/twip-bbb/lib/motors/direction.cpp -lgpiodcxx && ./main

// PID-like
g++ -o main main.cpp /home/debian/dev/twip-bbb/lib/mpu6050/mpu6050.cpp /home/debian/dev/twip-bbb/lib/mpu6050/kalman.cpp /home/debian/dev/twip-bbb/src/Controllers/pidlike.cpp /home/debian/dev/twip-bbb/lib/motors/pwm.cpp /home/debian/dev/twip-bbb/lib/motors/direction.cpp -lgpiodcxx && ./main

*/