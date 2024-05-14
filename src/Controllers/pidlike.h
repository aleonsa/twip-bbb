#ifndef PIDLIKE_H
#define PIDLIKE_H

class PIDLike
{
private:
    const double k1 = 2.7;
    // const double k2 = 4.281;
    const double k2 = 5.345;
    const double k3 = 1.1;
    bool firstRun;
    double L;
    double z;
    double z_dot;
    double previous_error;
    double derror;

public:
    // Constructor
    PIDLike(double L);

    // Calculate control signal
    double calculate(double error, double dt);
};

#endif /* PIDLIKE_H */