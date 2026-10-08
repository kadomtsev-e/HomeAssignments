#include "percolation.hpp"

#include <cstdlib>
#include <exception>
#include <iomanip>
#include <iostream>
#include <string>

int main(int argc, char* argv[])
{
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " DIMENSION TRIALS\n";
        return 1;
    }

    try {
        const std::size_t dimension = std::stoull(argv[1]);
        const std::size_t trials = std::stoull(argv[2]);
        PercolationStats stats(dimension, trials);
        stats.execute();
        std::cout << std::setprecision(10)
                  << "mean = " << stats.get_mean() << '\n'
                  << "stddev = " << stats.get_standard_deviation() << '\n'
                  << "95% confidence interval = [" << stats.get_confidence_low()
                  << ", " << stats.get_confidence_high() << "]\n";
    } catch (const std::exception& error) {
        std::cerr << error.what() << '\n';
        return 1;
    }
    return 0;
}
