#include "../lib/motors/pwm.h"
#include <iostream>
#include <unistd.h>

int main() {
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
    float step = 0.02;
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

        usleep(20 * 1000); // sleep for 20 milliseconds
    }

    return 0;
}
// g++ -o pwm_test2 /home/debian/dev/twip-bbb/tests/pwm_test2.cpp /home/debian/dev/twip-bbb/lib/motors/pwm.cpp && ./pwm_test2