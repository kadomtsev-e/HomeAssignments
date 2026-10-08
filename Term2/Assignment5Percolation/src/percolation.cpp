#include "percolation.hpp"

#include <algorithm>
#include <cmath>
#include <limits>
#include <numeric>
#include <stdexcept>

namespace {

std::size_t checked_site_count(const std::size_t dimension)
{
    if (dimension == 0) {
        throw std::invalid_argument("percolation dimension must be positive");
    }
    if (dimension > std::numeric_limits<std::size_t>::max() / dimension) {
        throw std::overflow_error("percolation grid is too large");
    }
    const std::size_t site_count = dimension * dimension;
    if (site_count > std::numeric_limits<std::size_t>::max() - 2) {
        throw std::overflow_error("percolation grid is too large");
    }
    return site_count;
}

std::uint64_t random_seed()
{
    std::random_device source;
    const std::uint64_t high = static_cast<std::uint64_t>(source()) << 32U;
    return high ^ static_cast<std::uint64_t>(source());
}

} // namespace

Percolation::DisjointSet::DisjointSet(const std::size_t size)
    : parent_(size)
    , sizes_(size, 1)
{
    std::iota(parent_.begin(), parent_.end(), std::size_t{0});
}

std::size_t Percolation::DisjointSet::find(std::size_t value)
{
    while (value != parent_[value]) {
        parent_[value] = parent_[parent_[value]];
        value = parent_[value];
    }
    return value;
}

void Percolation::DisjointSet::unite(const std::size_t left, const std::size_t right)
{
    std::size_t left_root = find(left);
    std::size_t right_root = find(right);
    if (left_root == right_root) {
        return;
    }
    if (sizes_[left_root] < sizes_[right_root]) {
        std::swap(left_root, right_root);
    }
    parent_[right_root] = left_root;
    sizes_[left_root] += sizes_[right_root];
}

bool Percolation::DisjointSet::connected(const std::size_t left, const std::size_t right)
{
    return find(left) == find(right);
}

Percolation::Percolation(const std::size_t dimension)
    : dimension_(dimension)
    , site_count_(checked_site_count(dimension))
    , virtual_top_(site_count_)
    , virtual_bottom_(site_count_ + 1)
    , open_(site_count_, false)
    , percolation_sets_(site_count_ + 2)
    , fullness_sets_(site_count_ + 1)
{
}

std::size_t Percolation::index(const std::size_t row, const std::size_t column) const
{
    if (row >= dimension_ || column >= dimension_) {
        throw std::out_of_range("percolation coordinates are outside the grid");
    }
    return row * dimension_ + column;
}

void Percolation::connect_if_open(
    const std::size_t site,
    const std::size_t row,
    const std::size_t column)
{
    const std::size_t neighbour = index(row, column);
    if (open_[neighbour]) {
        percolation_sets_.unite(site, neighbour);
        fullness_sets_.unite(site, neighbour);
    }
}

bool Percolation::open(const std::size_t row, const std::size_t column)
{
    const std::size_t site = index(row, column);
    if (open_[site]) {
        return false;
    }

    open_[site] = true;
    ++open_count_;

    if (row == 0) {
        percolation_sets_.unite(site, virtual_top_);
        fullness_sets_.unite(site, virtual_top_);
    }
    if (row + 1 == dimension_) {
        percolation_sets_.unite(site, virtual_bottom_);
    }
    if (row > 0) {
        connect_if_open(site, row - 1, column);
    }
    if (row + 1 < dimension_) {
        connect_if_open(site, row + 1, column);
    }
    if (column > 0) {
        connect_if_open(site, row, column - 1);
    }
    if (column + 1 < dimension_) {
        connect_if_open(site, row, column + 1);
    }
    return true;
}

bool Percolation::is_open(const std::size_t row, const std::size_t column) const
{
    return open_[index(row, column)];
}

bool Percolation::is_full(const std::size_t row, const std::size_t column)
{
    const std::size_t site = index(row, column);
    return open_[site] && fullness_sets_.connected(site, virtual_top_);
}

bool Percolation::percolates()
{
    return percolation_sets_.connected(virtual_top_, virtual_bottom_);
}

std::size_t Percolation::number_of_open_sites() const noexcept
{
    return open_count_;
}

std::size_t Percolation::dimension() const noexcept
{
    return dimension_;
}

PercolationStats::PercolationStats(const std::size_t dimension, const std::size_t trials)
    : PercolationStats(dimension, trials, random_seed())
{
}

PercolationStats::PercolationStats(
    const std::size_t dimension,
    const std::size_t trials,
    const std::uint64_t seed)
    : dimension_(dimension)
    , trials_(trials)
    , generator_(seed)
{
    static_cast<void>(checked_site_count(dimension));
    if (trials == 0) {
        throw std::invalid_argument("number of trials must be positive");
    }
}

double PercolationStats::get_mean() const noexcept
{
    return mean_;
}

double PercolationStats::get_standard_deviation() const noexcept
{
    return standard_deviation_;
}

double PercolationStats::get_confidence_low() const noexcept
{
    return confidence_low_;
}

double PercolationStats::get_confidence_high() const noexcept
{
    return confidence_high_;
}

void PercolationStats::execute()
{
    const std::size_t site_count = checked_site_count(dimension_);
    std::vector<std::size_t> order(site_count);
    std::vector<double> thresholds;
    thresholds.reserve(trials_);

    for (std::size_t trial = 0; trial < trials_; ++trial) {
        std::iota(order.begin(), order.end(), std::size_t{0});
        std::shuffle(order.begin(), order.end(), generator_);
        Percolation grid(dimension_);

        for (const std::size_t site : order) {
            grid.open(site / dimension_, site % dimension_);
            if (grid.percolates()) {
                thresholds.push_back(
                    static_cast<double>(grid.number_of_open_sites()) /
                    static_cast<double>(site_count));
                break;
            }
        }
    }

    mean_ = std::accumulate(thresholds.begin(), thresholds.end(), 0.0) /
        static_cast<double>(trials_);
    if (trials_ == 1) {
        standard_deviation_ = 0.0;
        confidence_low_ = mean_;
        confidence_high_ = mean_;
        return;
    }

    double squared_deviations = 0.0;
    for (const double threshold : thresholds) {
        const double difference = threshold - mean_;
        squared_deviations += difference * difference;
    }
    standard_deviation_ = std::sqrt(squared_deviations / static_cast<double>(trials_ - 1));
    const double margin = 1.96 * standard_deviation_ / std::sqrt(static_cast<double>(trials_));
    confidence_low_ = mean_ - margin;
    confidence_high_ = mean_ + margin;
}
