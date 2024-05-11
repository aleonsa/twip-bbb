#include "hgo.h"

HGO::HGO(double mu1, double mu2, double mu3, double L, double dt) : mu1(mu1), mu2(mu2), mu3(mu3), L(L), dt(dt) {
    t = 0;
    x1 = 0;
    x2 = 0;
    x3 = 0;
}

std::pair<std::vector<double>, std::vector<double>> HGO::operator()(const std::vector<double>& u_signal) {
    std::vector<double> y1_signal;
    std::vector<double> y2_signal;

    for (double u : u_signal) {
        double y1 = x2;
        double y2 = x3;

        double k1 = -(mu1/L)*(x1-u) + x2;
        double k2 = -(mu2/(L*L))*(x1-u) + x3;
        double k3 = -(mu3/(L*L*L))*(x1-u);

        x1 = x1 + k1*dt;
        x2 = x2 + k2*dt;
        x3 = x3 + k3*dt;

        t = t + dt;

        y1_signal.push_back(y1);
        y2_signal.push_back(y2);
    }

    return {y1_signal, y2_signal};
}