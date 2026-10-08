# Assignment 1: ODR and Global Variables

## Author
Egor Kadomtsev, Group 25.B81-mm

## Contacts
st116930@student.spbu.ru

## Description
This project solves the lecture 1 practice task about ODR, linkage, and global variables in headers.

It contains two corrected versions of the same broken mini-project:
- `extern_solution` fixes the issue with `extern` declarations in the header and one definition in a `.cpp` file.
- `inline_solution` fixes the issue with a C++17 `inline` variable in the header.

The explanation of the linker error and the answer to the extra theory question are provided in `answers.txt`.

## Build
```bash
make
```

## Run
```bash
./bin/odr_extern
./bin/odr_inline
```
