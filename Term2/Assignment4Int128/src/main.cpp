#include <iostream>

#include "expression.hpp"

int main() {
    const Int128 lhs("170141183460469231731687303715884105727");
    const Int128 rhs("-42");

    std::cout << "lhs = " << lhs << '\n';
    std::cout << "rhs = " << rhs << '\n';
    std::cout << "lhs + rhs = " << (lhs + rhs) << '\n';
    std::cout << "lhs / 3 = " << (lhs / Int128(3)) << '\n';

    const Add expr = Const(2) * Variable("x") + Const(1);
    const Int128 result = expr.eval({{"x", Int128(100)}});

    std::cout << "expr = " << expr << '\n';
    std::cout << "expr(x = 100) = " << result << '\n';

    return 0;
}
