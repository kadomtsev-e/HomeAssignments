# Assignment 3: AVL Tree

## Author
Egor Kadomtsev, Group 25.B81-mm

## Contacts
st116930@student.spbu.ru

## Description
This project implements an AVL tree over `int` values with manual memory management.

Supported operations:

- membership check with `contains`;
- insertion without duplicates;
- deletion with automatic rebalancing;
- size and emptiness queries;
- sorted traversal through `values()`.

The implementation keeps the tree balanced after insertions and removals using AVL rotations and frees all allocated memory in the destructor.

## Structure
- `include/AVLTree.hpp` - public AVL tree interface required by the assignment
- `src/AVLTree.hpp` - compatibility forwarding header
- `src/AVLTree.cpp` - AVL tree implementation
- `src/main.cpp` - small demo program
- `tests/test_avl_tree.cpp` - self-checking test suite

## Build
```bash
make
```

## Run
```bash
./bin/avl_demo
```

## Test
```bash
make test
```

The tests include deterministic randomized comparison with `std::set`.
Memory and undefined-behavior checks can be run with `make sanitize`.
