#ifndef HGO_H
#define HGO_H

#include <vector>

class HGO {
    private:
        double t, x1, x2, x3;
        double mu1, mu2, mu3;
        double L;
        double dt;

    public:
        HGO(double mu1, double mu2, double mu3, double L, double dt);
        std::pair<std::vector<double>, std::vector<double>> operator()(const std::vector<double>& u_signal);
};

#endif // HGO_H