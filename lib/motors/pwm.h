#ifndef PWM_H
#define PWM_H

#include <string>
#include <stdexcept> // for std::runtime_error

class PWM {
public:
    PWM(const std::string& pwm, const std::string& channel, const std::string& pin);

    void setPeriod(int period);
    void setDutyCycle(float dutyCycle);
    void enable();
    void disable();

private:
    std::string pwmPath;
    void writeToFile(const std::string& filename, const std::string& value);
};

class FileOpenException : public std::runtime_error {
public:
    FileOpenException(const std::string& filename)
        : std::runtime_error("Unable to open file: " + filename) {}
};

#endif // PWM_H