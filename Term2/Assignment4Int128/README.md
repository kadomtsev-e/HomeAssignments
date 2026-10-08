# Assignment: Int128 and Arithmetic Expressions

## Author
Egor Kadomtsev, Group 25.B81-mm

## Contacts
st116930@student.spbu.ru

## Description
This project implements:

- a signed `Int128` class with 128-bit two's-complement arithmetic;
- a hierarchy of arithmetic expressions evaluated over `Int128`.

Supported `Int128` features:

- default construction;
- construction from `int64_t` and `std::string_view`;
- explicit conversion to `int64_t` and `double`;
- decimal conversion with `str()`;
- arithmetic operators `+`, `-`, `*`, `/` and compound assignment variants;
- unary minus, comparisons, and stream output.

Supported expression nodes:

- `Const`
- `Variable`
- `Negate`
- `Add`
- `Subtract`
- `Multiply`
- `Divide`

Expressions support cloning, evaluation with a variable map, arithmetic composition through operators, and stream output.

## Structure
- `src/int128.*` - implementation of the 128-bit integer type
- `src/expression.*` - expression tree hierarchy over `Int128`
- `src/main.cpp` - small demo program
- `tests/test_int128.cpp` - self-checking test suite

## Build
```bash
make
```

## Run
```bash
./bin/int128_demo
```

## Test
```bash
make test
```

The test suite includes deterministic differential arithmetic checks against the compiler's
native 128-bit integer extension. Run memory and UB checks with `make sanitize`.
