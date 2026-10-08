#pragma once

#include <cstddef>
#include <cstdint>
#include <random>
#include <vector>

class Percolation
{
public:
    explicit Percolation(std::size_t dimension);

    bool open(std::size_t row, std::size_t column);
    [[nodiscard]] bool is_open(std::size_t row, std::size_t column) const;
    [[nodiscard]] bool is_full(std::size_t row, std::size_t column);
    [[nodiscard]] bool percolates();
    [[nodiscard]] std::size_t number_of_open_sites() const noexcept;
    [[nodiscard]] std::size_t dimension() const noexcept;

private:
    class DisjointSet
    {
    public:
        explicit DisjointSet(std::size_t size);
        std::size_t find(std::size_t value);
        void unite(std::size_t left, std::size_t right);
        bool connected(std::size_t left, std::size_t right);

    private:
        std::vector<std::size_t> parent_;
        std::vector<std::size_t> sizes_;
    };

    [[nodiscard]] std::size_t index(std::size_t row, std::size_t column) const;
    void connect_if_open(std::size_t site, std::size_t row, std::size_t column);

    std::size_t dimension_;
    std::size_t site_count_;
    std::size_t open_count_ = 0;
    std::size_t virtual_top_;
    std::size_t virtual_bottom_;
    std::vector<bool> open_;
    DisjointSet percolation_sets_;
    DisjointSet fullness_sets_;
};

struct PercolationStats
{
    PercolationStats(std::size_t dimension, std::size_t trials);
    PercolationStats(std::size_t dimension, std::size_t trials, std::uint64_t seed);

    [[nodiscard]] double get_mean() const noexcept;
    [[nodiscard]] double get_standard_deviation() const noexcept;
    [[nodiscard]] double get_confidence_low() const noexcept;
    [[nodiscard]] double get_confidence_high() const noexcept;

    void execute();

private:
    std::size_t dimension_;
    std::size_t trials_;
    std::mt19937_64 generator_;
    double mean_ = 0.0;
    double standard_deviation_ = 0.0;
    double confidence_low_ = 0.0;
    double confidence_high_ = 0.0;
};
