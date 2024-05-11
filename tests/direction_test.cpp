#include "../lib/motors/direction.h"
#include <thread>
#include <chrono>

int main() {
    Motor motor1("gpiochip2", 24, "gpiochip1", 29);
    Motor motor2("gpiochip1", 1, "gpiochip1", 5);

    while (true) {
        // Set the direction of the motors
        motor1.forward();
        motor2.backward();

        std::this_thread::sleep_for(std::chrono::seconds(1));

        motor1.backward();
        motor2.forward();

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return 0;
}