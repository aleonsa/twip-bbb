#ifndef PID_H
#define PID_H

class PID {
private:
    double Kp, Ki, Kd;
    double integral;
    double previous_error;

public:
    // Constructor
    PID(double Kp, double Ki, double Kd);

    // Calculate control signal
    double calculate(double error, double dt);
};

#endif /* PID_H */