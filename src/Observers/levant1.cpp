#include "levant1.h"
#include <cmath>

Levant1::Levant1(double L, double z0, double z1)
    : L(L), z0(z0), z1(z1), k1(1.5), k2(1.1) {}

std::pair<double, double> Levant1::levant1(double u) {
    double e = z0 - u;
    double dz0 = -k1 * std::sqrt(L) * Csign(e, 0.5) + z1;
    double dz1 = -k2 * L * Csign(e, 0);
    return std::make_pair(dz0, dz1);
}

double Levant1::Csign(double e, double alpha) {
    return std::pow(std::abs(e), alpha) * (e > 0 ? 1 : -1);
}

void Levant1::euler_integration_1st_order(const std::vector<double>& u_signal, double dt) {
    dz0_signal.clear();
    dz1_signal.clear();
    for (double u : u_signal) {
        auto [dz0, dz1] = levant1(u);
        z0 = z0 + dz0 * dt;
        z1 = z1 + dz1 * dt;
        dz0_signal.push_back(dz0);
        dz1_signal.push_back(dz1);
    }
}

std::vector<double> Levant1::get_dz0_signal() {
    return dz0_signal;
}

std::vector<double> Levant1::get_dz1_signal() {
    return dz1_signal;
}