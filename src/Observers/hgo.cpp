#include "hgo.h"

HighGainObserver::HighGainObserver() : x1(0), x2(0), x3(0) {}

std::pair<double, double> HighGainObserver::hgo(double u, double L, double dt) {
    double y1 = x2;
    double y2 = x3;

    double k1 = -(mu1/L)*(x1-u) + x2;
    double k2 = -(mu2/(L*L))*(x1-u) + x3;
    double k3 = -(mu3/(L*L*L))*(x1-u);

    x1 = x1 + k1*dt;
    x2 = x2 + k2*dt;
    x3 = x3 + k3*dt;

    return {y1, y2};
}