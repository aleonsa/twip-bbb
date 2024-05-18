#include "levant1.h"
#include <cmath>

Levant1::Levant1(double L, double dt) : L(L), dt(dt), z0(0), z1(0) {}

void Levant1::update(double u) {
    double e = z0 - u;
    double dz0 = -k1 * std::pow(L, 0.5) * std::copysign(std::pow(std::abs(e), 0.5), e) + z1;
    double dz1 = -k2 * L * std::copysign(std::pow(std::abs(e), 0), e);

    z0 += dz0 * dt;
    z1 += dz1 * dt;
}

double Levant1::getZ0() const { return z0; }
double Levant1::getZ1() const { return z1; }