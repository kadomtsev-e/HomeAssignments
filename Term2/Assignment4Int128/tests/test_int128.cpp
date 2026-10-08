#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <random>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>

#include "../src/expression.hpp"

namespace {

__extension__ using Native128 = __int128;

void require(bool condition, const std::string& message) {
    if (!condition) {
        std::cerr << "Test failed: " << message << '\n';
        std::exit(1);
    }
}

void requireEqual(
    const Int128& actual,
    const std::string& expected,
    const std::string& message) {
    require(actual.str() == expected, message + ": expected " + expected + ", got " + actual.str());
}

std::string nativeString(Native128 value) {
    if (value == 0) {
        return "0";
    }

    const bool negative = value < 0;
    __extension__ unsigned __int128 magnitude = negative
        ? static_cast<unsigned __int128>(-(value + 1)) + 1
        : static_cast<unsigned __int128>(value);
    std::string result;
    while (magnitude != 0) {
        result.push_back(static_cast<char>('0' + magnitude % 10));
        magnitude /= 10;
    }
    if (negative) {
        result.push_back('-');
    }
    std::reverse(result.begin(), result.end());
    return result;
}

} // namespace

int main() {
    require(Int128().str() == "0", "default constructor creates zero");
    require(Int128(42).str() == "42", "int64_t constructor handles positive numbers");
    require(Int128(-42).str() == "-42", "int64_t constructor handles negative numbers");

    requireEqual(
        Int128("170141183460469231731687303715884105727"),
        "170141183460469231731687303715884105727",
        "max value roundtrip");
    requireEqual(
        Int128("-170141183460469231731687303715884105728"),
        "-170141183460469231731687303715884105728",
        "min value roundtrip");

    requireEqual(
        Int128("18446744073709551616") + Int128(5),
        "18446744073709551621",
        "addition across 64-bit boundary");
    requireEqual(
        Int128("18446744073709551616") - Int128(1),
        "18446744073709551615",
        "subtraction across 64-bit boundary");
    requireEqual(
        Int128(-7) + Int128(3),
        "-4",
        "signed addition");

    requireEqual(
        Int128("1234567890123456789") * Int128("1000000"),
        "1234567890123456789000000",
        "large multiplication");
    requireEqual(
        Int128(-7) * Int128(9),
        "-63",
        "signed multiplication");

    requireEqual(
        Int128("1208925819614629174706176") / Int128("65536"),
        "18446744073709551616",
        "large division");
    requireEqual(
        Int128("-170141183460469231731687303715884105728") / Int128(2),
        "-85070591730234615865843651857942052864",
        "minimum value division stays representable");
    requireEqual(
        Int128("-170141183460469231731687303715884105728") * Int128(1),
        "-170141183460469231731687303715884105728",
        "minimum value multiplication by one is stable");
    requireEqual(
        Int128(-7) / Int128(3),
        "-2",
        "division truncates toward zero");

    require(static_cast<int64_t>(Int128(-1)) == -1, "cast to int64_t keeps low 64 bits");
    require(
        std::fabs(static_cast<double>(Int128("9223372036854775808")) - std::ldexp(1.0, 63)) < 0.5,
        "cast to double handles powers of two");

    require(Int128(-10) < Int128(5), "signed comparison works");
    require(Int128("10000000000000000000") > Int128("9999999999999999999"), "large comparison works");
    require(Int128(42) != Int128(41), "inequality works");
    require(Int128("+000123").str() == "123", "string constructor handles leading zeros and plus sign");

    bool invalid_string_thrown = false;
    try {
        static_cast<void>(Int128("12x3"));
    } catch (const std::invalid_argument&) {
        invalid_string_thrown = true;
    }
    require(invalid_string_thrown, "invalid characters in string constructor throw invalid_argument");

    bool overflow_thrown = false;
    try {
        static_cast<void>(Int128("170141183460469231731687303715884105728"));
    } catch (const std::out_of_range&) {
        overflow_thrown = true;
    }
    require(overflow_thrown, "positive overflow in string constructor throws out_of_range");

    bool negative_overflow_thrown = false;
    try {
        static_cast<void>(Int128("-170141183460469231731687303715884105729"));
    } catch (const std::out_of_range&) {
        negative_overflow_thrown = true;
    }
    require(negative_overflow_thrown, "negative overflow in string constructor throws out_of_range");

    bool division_by_zero_thrown = false;
    try {
        static_cast<void>(Int128(10) / Int128(0));
    } catch (const std::domain_error&) {
        division_by_zero_thrown = true;
    }
    require(division_by_zero_thrown, "division by zero throws domain_error");

    const Add expr = Const(2) * Variable("x") + Const(1);
    requireEqual(expr.eval({{"x", Int128(100)}}), "201", "expression evaluation");

    const Negate composite = -(Variable("x") - Const(5));
    requireEqual(composite.eval({{"x", Int128(2)}}), "3", "unary negation on expression");

    std::unique_ptr<Expression> cloned(expr.clone());
    requireEqual(cloned->eval({{"x", Int128(7)}}), "15", "clone preserves expression");

    bool expression_division_by_zero_thrown = false;
    try {
        static_cast<void>((Variable("x") / Const(0)).eval({{"x", Int128(1)}}));
    } catch (const std::domain_error&) {
        expression_division_by_zero_thrown = true;
    }
    require(expression_division_by_zero_thrown, "expression division by zero propagates domain_error");

    std::ostringstream out;
    out << expr;
    require(out.str() == "((2 * x) + 1)", "expression stream output is stable");

    std::ostringstream int_out;
    int_out << Int128("-12345678901234567890");
    require(int_out.str() == "-12345678901234567890", "Int128 stream output is stable");

    std::mt19937_64 generator(0x128U);
    std::uniform_int_distribution<int64_t> distribution(
        -1000000000000000000LL,
        1000000000000000000LL);
    for (int iteration = 0; iteration < 5000; ++iteration) {
        const int64_t left = distribution(generator);
        int64_t right = distribution(generator);
        if (right == 0) {
            right = 1;
        }

        const Native128 nativeLeft = left;
        const Native128 nativeRight = right;
        const Int128 actualLeft(left);
        const Int128 actualRight(right);
        requireEqual(actualLeft + actualRight, nativeString(nativeLeft + nativeRight), "random addition");
        requireEqual(actualLeft - actualRight, nativeString(nativeLeft - nativeRight), "random subtraction");
        requireEqual(actualLeft * actualRight, nativeString(nativeLeft * nativeRight), "random multiplication");
        requireEqual(actualLeft / actualRight, nativeString(nativeLeft / nativeRight), "random division");
    }

    const Add copiedExpression = expr;
    Add expressionToMove(Const(3), Variable("x"));
    const Add movedExpression = std::move(expressionToMove);
    requireEqual(copiedExpression.eval({{"x", Int128(5)}}), "11", "copied expression owns its nodes");
    requireEqual(movedExpression.eval({{"x", Int128(4)}}), "7", "moved expression owns its nodes");

    std::cout << "All Int128 tests passed.\n";
    return 0;
}
