#include "../lib/motors/pwm.h"
#include "../lib/motors/direction.h"
#include <iostream>
#include <unistd.h>
#include <thread>
#include <chrono>

int main() {
    // MOTOR'S DIRECTION INIT
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

    float dutyCycle = 0.02;
    float step = 0.01;
    float min = 0.02;
    float max = 0.98;

    std::cout << "Starting PWM duty cycle adjustments..." << std::endl;


    while (true) {
        pwmA.setDutyCycle(dutyCycle); // duty cycle as a fraction of the period
        pwmB.setDutyCycle(dutyCycle); // duty cycle as a fraction of the period

        std::cout << "Duty cycle: " << dutyCycle << std::endl;

        dutyCycle += step;
        if (dutyCycle >= max || dutyCycle <= min) {
            step = -step;
        }
        if(step>=0){
            motor1.backward();
            motor2.backward();
        } else{
            motor1.forward();
            motor2.forward();
        }
        

        usleep(100 * 1000); // sleep for 100 milliseconds
    }

    return 0;
}
// g++ -o pwm_motors_test /home/debian/dev/twip-bbb/tests/pwm_motors_test.cpp /home/debian/dev/twip-bbb/lib/motors/pwm.cpp   /home/debian/dev/twip-bbb//lib/motors/direction.cpp -lgpiodcxx && ./pwm_motors_test