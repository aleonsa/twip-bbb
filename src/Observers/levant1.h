#ifndef LEVANT1_H
#define LEVANT1_H

#include <vector>

class Levant1 {
public:
    Levant1(double L, double z0, double z1);
    void euler_integration_1st_order(const std::vector<double>& u_signal, double dt);
    std::vector<double> get_dz0_signal();
    std::vector<double> get_dz1_signal();

private:
    double L;
    double z0;
    double z1;
    double k1;
    double k2;
    std::vector<double> dz0_signal;
    std::vector<double> dz1_signal;
    std::pair<double, double> levant1(double u);
    double Csign(double e, double alpha);
};

#endif // LEVANT1_H