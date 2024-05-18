#include <utility>

class HighGainObserver {
private:
    double x1, x2, x3;
    const double mu1 = 3.0, mu2 = 3.0, mu3 = 1.0;

public:
    HighGainObserver();

    std::pair<double, double> hgo(double u, double L, double dt);
};