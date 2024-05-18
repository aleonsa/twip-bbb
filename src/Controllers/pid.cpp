#include "pid.h"

PID::PID(double Kp, double Ki, double Kd) : Kp(Kp), Ki(Ki), Kd(Kd), ierr(0), firstRun(true) {}

double PID::calculate(double error, double derror,double dt) {
    ierr += error * dt;
    if(firstRun){
        derror = 0;
        firstRun = false;
    } else{
        // derror = (error - previous_error) / dt; // Store the derivative
    }
    
    double output = Kp*error + Ki*ierr + Kd*derror;
    return output;
}

double PID::getIntegral() const {
    return ierr;
}

double PID::getDerivative() const {
    return 0;
}