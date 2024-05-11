#include "direction.h"

Motor::Motor(const std::string& chipName1, unsigned int line1Offset, const std::string& chipName2, unsigned int line2Offset) {
    gpiod::chip chip1(chipName1);
    gpiod::chip chip2(chipName2);
    line1 = chip1.get_line(line1Offset);
    line2 = chip2.get_line(line2Offset);

    gpiod::line_request request;
    request.consumer = "Motor";
    request.request_type = gpiod::line_request::DIRECTION_OUTPUT;

    line1.request(request);
    line2.request(request);
}

void Motor::forward() {
    set(1, 0);
}

void Motor::backward() {
    set(0, 1);
}

void Motor::set(int value1, int value2) {
    line1.set_value(value1);
    line2.set_value(value2);
}