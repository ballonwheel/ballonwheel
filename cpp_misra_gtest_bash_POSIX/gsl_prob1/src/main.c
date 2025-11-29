#include <iostream>
#include <gsl/gsl_cdf.h> // for cumulative distribution functions

int main() {
    double x = 1.96;  // example z-score
    double p = gsl_cdf_ugaussian_P(x); // cumulative prob for standard normal

    std::cout << "P(Z ≤ " << x << ") = " << p << std::endl;
    return 0;
}
