#include <gpiod.hpp>
#include <chrono>
#include <thread>
#include <vector>

int main() {
    const std::string chipName = "gpiochip2";
    std::vector<unsigned int> lineOffsets = {24};

    gpiod::chip chip(chipName);
    auto lines = chip.get_lines(lineOffsets);

    gpiod::line_request request;
    request.consumer = "externLED.cpp";
    request.request_type = gpiod::line_request::DIRECTION_OUTPUT;

    lines.request(request);

    int state = 0;
    while (true) {
        lines.set_values({state});
        state = ~state;
        std::this_thread::sleep_for(std::chrono::milliseconds(250));
    }

    return 0;
}

// g++ gpio_test.cpp -o gpio_test -lgpiodcxx && ./gpio_test