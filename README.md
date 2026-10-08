# C++ Home Assignments

## Student Information
- **Name**: Egor Kadomtsev
- **Group**: 25.B81-mm
- **Email**: st116930@student.spbu.ru
- **University ID**: st116930

## Description
This repository contains home assignments for the C++ course at Saint Petersburg State University (SPbU).

## Structure
- [Term 1](Term1/)
  - [Assignment 1](Term1/Assignment1/) - Hello World loop and user input
  - [Assignment 2a](Term1/Assignment2a/) - Binary file reversal
  - [Assignment 2b](Term1/Assignment2b/) - RPN calculator with custom stack
  - [Assignment 3](Term1/Assignment3/) - Transformers class hierarchy
  - [Assignment 5](Term1/Assignment5/) - Matrix template and Rational class
- [Term 2](Term2/)
  - [Assignment 1](Term2/Assignment1/) - ODR, linkage, and inline variables
  - [Assignment 2](Term2/Assignment2Calc/) - Single-register calculator with trigonometry
  - [Assignment 3](Term2/Assignment3AVL/) - AVL tree with manual memory management
  - [Assignment 4](Term2/Assignment4Int128/) - Signed Int128 and expression trees
  - [Assignment 5](Term2/Assignment5Percolation/) - Percolation Monte Carlo simulation
  - [Practice 13 May](Term2/Practice_13_05/) - Yandex office tour with raw pointers

## Build and test Term 2

All second-semester projects use C++17 and can be checked together:

```bash
make -C Term2 cleanall
make -C Term2 all
make -C Term2 test
```

AddressSanitizer and UndefinedBehaviorSanitizer checks are available with:

```bash
make -C Term2 sanitize
```
