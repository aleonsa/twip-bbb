#include "pidlike.h"
#include <cmath>

PIDLike::PIDLike(double L) : L(L), firstRun(true), z(0), z_dot(0){}

double PIDLike::calculate(double error, double derror, double dt) {
    if(firstRun){
        derror = 0;
        firstRun = false;
    } else{
        // derror = (error - previous_error) / dt; // Store the derivative
        z_dot = k3 * L * (error >= 0 ? 1 : -1);
        z += z_dot * dt;
    }

    double upid = k1 * std::pow(L, 2.0 / 3.0) * std::abs(std::pow(std::abs(error), 1.0 / 3.0)) * (error >= 0 ? 1 : -1)
                + k2 * std::pow(L, 1.0 / 2.0) * std::abs(std::pow(std::abs(derror), 1.0 / 2.0)) * (derror >= 0 ? 1 : -1)
                + z;

    //previous_error = error;
    return upid;
}