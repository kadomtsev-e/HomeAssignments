#include "percolation.hpp"

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>

namespace {

void require(const bool condition, const std::string& message)
{
    if (!condition) {
        std::cerr << "Test failed: " << message << '\n';
        std::exit(1);
    }
}

void require_near(const double actual, const double expected, const std::string& message)
{
    require(std::fabs(actual - expected) < 1e-12, message);
}

} // namespace

int main()
{
    Percolation single(1);
    require(!single.percolates(), "closed 1x1 grid does not percolate");
    require(single.number_of_open_sites() == 0, "new grid has no open sites");
    require(single.open(0, 0), "opening a closed site succeeds");
    require(!single.open(0, 0), "opening an open site is not counted twice");
    require(single.number_of_open_sites() == 1, "repeated opening keeps count");
    require(single.is_full(0, 0), "open 1x1 site is full");
    require(single.percolates(), "open 1x1 grid percolates");

    Percolation grid(3);
    grid.open(0, 0);
    grid.open(1, 1);
    grid.open(2, 2);
    require(!grid.percolates(), "diagonal cells are not connected");
    grid.open(1, 0);
    grid.open(2, 0);
    require(grid.percolates(), "orthogonal top-to-bottom path percolates");

    Percolation backwash(3);
    backwash.open(0, 0);
    backwash.open(1, 0);
    backwash.open(2, 0);
    backwash.open(2, 2);
    require(backwash.percolates(), "test grid percolates");
    require(!backwash.is_full(2, 2), "bottom-only site is not full after percolation");

    bool bad_coordinate = false;
    try {
        grid.open(3, 0);
    } catch (const std::out_of_range&) {
        bad_coordinate = true;
    }
    require(bad_coordinate, "out-of-range coordinates are rejected");

    PercolationStats one_trial(1, 1, 7);
    one_trial.execute();
    require_near(one_trial.get_mean(), 1.0, "1x1 threshold is one");
    require_near(one_trial.get_standard_deviation(), 0.0, "single-trial deviation is documented as zero");
    require_near(one_trial.get_confidence_low(), 1.0, "single-trial low endpoint equals mean");
    require_near(one_trial.get_confidence_high(), 1.0, "single-trial high endpoint equals mean");

    PercolationStats first(20, 100, 123456);
    PercolationStats second(20, 100, 123456);
    first.execute();
    second.execute();
    require_near(first.get_mean(), second.get_mean(), "controlled seeds reproduce results");
    require(first.get_mean() > 0.45 && first.get_mean() < 0.75, "estimated threshold is reasonable");
    require(first.get_standard_deviation() > 0.0, "multiple trials produce a sample deviation");
    require(first.get_confidence_low() < first.get_mean(), "confidence low is below mean");
    require(first.get_confidence_high() > first.get_mean(), "confidence high is above mean");

    const double old_mean = first.get_mean();
    first.execute();
    require(first.get_mean() > 0.0 && first.get_mean() <= 1.0, "repeated execute computes fresh valid trials");
    require(old_mean != first.get_mean(), "repeated execute advances the random generator");

    bool bad_dimension = false;
    try {
        static_cast<void>(PercolationStats(0, 1));
    } catch (const std::invalid_argument&) {
        bad_dimension = true;
    }
    require(bad_dimension, "zero dimension is rejected");

    bool bad_trials = false;
    try {
        static_cast<void>(PercolationStats(1, 0));
    } catch (const std::invalid_argument&) {
        bad_trials = true;
    }
    require(bad_trials, "zero trials is rejected");

    std::cout << "All percolation tests passed.\n";
    return 0;
}
