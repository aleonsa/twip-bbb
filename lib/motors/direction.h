#ifndef DIRECTION_H
#define DIRECTION_H

#include <gpiod.hpp>

class Motor {
public:
    Motor(const std::string& chipName1, unsigned int line1, const std::string& chipName2, unsigned int line2);
    void forward();
    void backward();
private:
    void set(int value1, int value2);
    gpiod::line line1;
    gpiod::line line2;
};

#endif // DIRECTION_H