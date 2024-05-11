#include "pwm.h"
#include <fstream>
#include <iostream>
#include <cmath>
#include <cstdlib>

// Constructor: Initializes the PWM object and sets the duty cycle to 0
PWM::PWM(const std::string& pwm, const std::string& channel, const std::string& pin)
    : pwmPath("/dev/bone/pwm/" + pwm + "/" + channel) {
    // Run the command to activate the pin as PWM
    std::string command = "config-pin " + pin + " pwm";
    system(command.c_str());
    // Initialize the duty cycle to 0
    setDutyCycle(0);
}

// Sets the period of the PWM signal
void PWM::setPeriod(int period) {
    writeToFile("period", std::to_string(period));
}

// Sets the duty cycle of the PWM signal
void PWM::setDutyCycle(float dutyCycle) {
    writeToFile("duty_cycle", std::to_string(static_cast<int>(round(1000000 * dutyCycle))));
}

// Enables the PWM signal
void PWM::enable() {
    writeToFile("enable", "1");
}

// Disables the PWM signal
void PWM::disable() {
    writeToFile("enable", "0");
}

// Writes a value to a file at the specified path
void PWM::writeToFile(const std::string& filename, const std::string& value) {
    std::ofstream file(pwmPath + "/" + filename);
    // If the file is open, write the value and close the file
    if (file.is_open()) {
        file << value;
        file.close();
    } else {
        // If the file can't be opened, throw a FileOpenException
        throw FileOpenException(pwmPath + "/" + filename);
    }
}