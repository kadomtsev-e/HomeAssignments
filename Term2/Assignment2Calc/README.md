# Assignment 2: Calculator

## Author
Egor Kadomtsev, Group 25.B81-mm

## Contacts
st116930@student.spbu.ru

## Description
This project implements a single-register calculator for Term 2 Assignment 2.

Base functionality:

- setting the register from a decimal number;
- binary operations `+`, `-`, `*`, `/`, `%`, `^`;
- unary operations `_` and `SQRT`.

Group B81 extension:

- trigonometric operations `SIN`, `COS`, `TAN`, `CTN`;
- inverse trigonometric operations `ASIN`, `ACOS`, `ATAN`, `ACTN`;
- mode switching with `RAD` and `DEG`.

Bonus functionality:

- left folds for all binary operations using syntax like `(+) 1 2 3` or `(^) 2 3`.

The implementation provides both interfaces:

- `double process_line(double current, const std::string & line);`
- `double process_line(double current, bool & rad_on, const std::string & line);`

The second overload is used by the interactive program, while the first one keeps the original single-function interface required by the bonus task.

## Structure
- `src/calc.*` - calculator parsing and evaluation logic
- `src/main.cpp` - interactive command-line interface
- `tests/test_calc.cpp` - self-checking test suite

## Build
```bash
make
```

## Run
```bash
./bin/calc_demo
```

## Test
```bash
make test
```

Run the same suite with AddressSanitizer and UndefinedBehaviorSanitizer using `make sanitize`.
