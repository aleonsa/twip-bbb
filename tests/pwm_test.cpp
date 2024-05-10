#include <iostream>
#include <fstream>
#include <cmath>
#include <unistd.h>

int main() {
    // Fade time in ms
    int ms = 20;

    // Period in ns
    int pwmPeriod = 1000000;

    // PWM to use
    std::string pwm = "1";

    // Channel to use
    std::string channel = "a";

    // Path to the  device
    std::string PWMPATH = "/dev/bone/pwm/" + pwm + "/" + channel;

    // Step size
    float step = 0.02;

    // Dimmest value
    float min = 0.02;

    // Brightest value
    float max = 1;

    // Current brightness
    float brightness = min;

    // Open the period file and write the period
    std::ofstream f;
    f.open(PWMPATH + "/period");
    if (f.is_open()) {
        f << pwmPeriod;
        f.close();
    } else {
        std::cout << "Unable to open file";
    }

    // Open the enable file and write '1'
    f.open(PWMPATH + "/enable");
    if (f.is_open()) {
        f << "1";
        f.close();
    } else {
        std::cout << "Unable to open file";
    }

    // Loop indefinitely
    while (true) {
        // Open the duty_cycle file
        f.open(PWMPATH + "/duty_cycle");
        if (f.is_open()) {
            // Seek to the beginning of the file
            f.seekp(0);

            // Write the current duty cycle
            f << static_cast<int>(round(pwmPeriod * brightness)) << std::flush;

            // Close the file
            f.close();

            // Update the brightness
            brightness += step;

            // If we've reached the min or max, reverse the direction
            if (brightness >= max || brightness <= min) {
                step = -1 * step;
            }

            // Sleep for the specified time
            usleep(ms * 1000);
        } else {
            std::cout << "Unable to open file";
        }
    }

    return 0;
}

// g++ -o pwm_test pwm_test.cpp && ./pwm_test