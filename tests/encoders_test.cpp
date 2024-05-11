#include <gpiod.hpp>
#include <iostream>
#include <thread>

class Encoder {
public:
    Encoder(const std::string& chipNameA, unsigned int lineA, const std::string& chipNameB, unsigned int lineB);
    void monitor();
private:
    gpiod::line lineA;
    gpiod::line lineB;
    int pulseCount;
};

Encoder::Encoder(const std::string& chipNameA, unsigned int lineAOffset, const std::string& chipNameB, unsigned int lineBOffset)
    : pulseCount(0) {
    gpiod::chip chipA(chipNameA);
    gpiod::chip chipB(chipNameB);
    lineA = chipA.get_line(lineAOffset);
    lineB = chipB.get_line(lineBOffset);

    gpiod::line_request request;
    request.consumer = "Encoder";
    request.request_type = gpiod::line_request::EVENT_BOTH_EDGES;

    lineA.request(request);
    lineB.request(request);
}

void Encoder::monitor() {
    int lastStateA = lineA.get_value();
    int lastStateB = lineB.get_value();
    int lastDegrees = 0;

    while (true) {
        int stateA = lineA.get_value();
        int stateB = lineB.get_value();

        if (stateA != lastStateA || stateB != lastStateB) {
            // State has changed
            if (stateA != stateB) {
                // Different states indicate one direction
                pulseCount++;
            } else {
                // Same states indicate the other direction
                pulseCount--;
            }

            int degrees = pulseCount * 4;
            if (degrees != lastDegrees) {
                std::cout << "Total rotation: " << degrees << " degrees\n";
                lastDegrees = degrees;
            }

            lastStateA = stateA;
            lastStateB = stateB;
        }

        std::this_thread::sleep_for(std::chrono::microseconds(10));  // Adjust as needed
    }
}

int main() {
    Encoder encoder1("gpiochip2", 4, "gpiochip2", 3);
    // Encoder encoder2("gpiochip0", 26, "gpiochip1", 12);

    std::thread t1([&encoder1]() { encoder1.monitor(); });
    // std::thread t2([&encoder2]() { encoder2.monitor(); });

    t1.join();
    // t2.join();

    return 0;
}

// g++ encoders_test.cpp -o encoders_test -lgpiodcxx && ./encoders_test