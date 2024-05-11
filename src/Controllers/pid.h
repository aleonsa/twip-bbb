#ifndef PID_H
#define PID_H

class PID
{
private:
    double Kp, Ki, Kd;
    double ierr;
    double previous_error;
    double derror;
    bool firstRun;

public:
    // Constructor
    PID(double Kp, double Ki, double Kd);
    // Calculate control signal
    double calculate(double error, double dt);
    // get derivative error
    double getDerivative() const;
    // get integral error
    double getIntegral() const;
};

#endif /* PID_H */