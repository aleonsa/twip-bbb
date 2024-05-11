#include "pid.h"

PID::PID(double Kp, double Ki, double Kd) : Kp(Kp), Ki(Ki), Kd(Kd), integral(0), previous_error(0) {}

double PID::calculate(double error, double dt) {
    integral += error * dt;
    double derivative = (error - previous_error) / dt;
    // try observers to derivate
    double output = Kp*error + Ki*integral + Kd*derivative;
    previous_error = error;
    return output;
}