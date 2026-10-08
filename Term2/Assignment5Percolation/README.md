# Assignment 5: Percolation

## Author
Egor Kadomtsev, Group 25.B81-mm

## Description

This project models site percolation on an `N` by `N` grid and estimates the threshold with
Monte Carlo experiments. Connectivity uses disjoint sets with path compression and union by
size. A second disjoint-set structure prevents backwash when `is_full()` is queried.

`PercolationStats::execute()` calculates the mean, sample standard deviation, and 95% confidence
interval. For one trial the sample deviation is undefined mathematically; this implementation
reports deviation `0` and the degenerate interval `[mean, mean]`.

The required two-argument constructor uses a nondeterministic seed. The additional three-argument
constructor accepts a seed for reproducible tests without changing the required interface.

## Build and run

```bash
make
./bin/percolation 20 100
```

## Test

```bash
make test
make sanitize
```
