#ifndef LEVANT1_H
#define LEVANT1_H

#include <vector>

class Levant1 {
public:
    Levant1(double L, double dt);

    void update(double u);
    double getZ0() const;
    double getZ1() const;
    std::vector<double> eulerIntegration(const std::vector<double>& u_signal);

private:
    const double k1 = 1.5;
    const double k2 = 1.1;
    const double L;
    const double dt;
    double z0;
    double z1;
};

#endif // LEVANT1_H