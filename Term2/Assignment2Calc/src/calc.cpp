#include "calc.hpp"

#include <algorithm>
#include <cctype>
#include <cmath>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace {

constexpr std::size_t kMaxDecimalDigits = 10;
constexpr double kPi = 3.14159265358979323846264338327950288;
constexpr double kEpsilon = 1e-12;

enum class Op {
    Invalid,
    Set,
    Add,
    Sub,
    Mul,
    Div,
    Rem,
    Neg,
    Pow,
    Sqrt,
    Sin,
    Cos,
    Tan,
    Ctn,
    Asin,
    Acos,
    Atan,
    Actn,
    Rad,
    Deg
};

std::string trim(const std::string& text)
{
    std::size_t begin = 0;
    while (begin < text.size() && std::isspace(static_cast<unsigned char>(text[begin])) != 0) {
        ++begin;
    }

    std::size_t end = text.size();
    while (end > begin && std::isspace(static_cast<unsigned char>(text[end - 1])) != 0) {
        --end;
    }

    return text.substr(begin, end - begin);
}

std::vector<std::string> split(const std::string& text)
{
    std::istringstream input(text);
    std::vector<std::string> tokens;
    for (std::string token; input >> token; ) {
        tokens.push_back(token);
    }
    return tokens;
}

std::string join_suffix(const std::vector<std::string>& tokens, std::size_t from)
{
    std::ostringstream out;
    for (std::size_t index = from; index < tokens.size(); ++index) {
        if (index != from) {
            out << ' ';
        }
        out << tokens[index];
    }
    return out.str();
}

bool is_binary(const Op op)
{
    switch (op) {
        case Op::Set:
        case Op::Add:
        case Op::Sub:
        case Op::Mul:
        case Op::Div:
        case Op::Rem:
        case Op::Pow:
            return true;
        default:
            return false;
    }
}

bool is_unary(const Op op)
{
    switch (op) {
        case Op::Neg:
        case Op::Sqrt:
        case Op::Sin:
        case Op::Cos:
        case Op::Tan:
        case Op::Ctn:
        case Op::Asin:
        case Op::Acos:
        case Op::Atan:
        case Op::Actn:
            return true;
        default:
            return false;
    }
}

bool is_mode_switch(const Op op)
{
    return op == Op::Rad || op == Op::Deg;
}

std::string op_name(const Op op)
{
    switch (op) {
        case Op::Add:
            return "+";
        case Op::Sub:
            return "-";
        case Op::Mul:
            return "*";
        case Op::Div:
            return "/";
        case Op::Rem:
            return "%";
        case Op::Neg:
            return "_";
        case Op::Pow:
            return "^";
        case Op::Sqrt:
            return "SQRT";
        case Op::Sin:
            return "SIN";
        case Op::Cos:
            return "COS";
        case Op::Tan:
            return "TAN";
        case Op::Ctn:
            return "CTN";
        case Op::Asin:
            return "ASIN";
        case Op::Acos:
            return "ACOS";
        case Op::Atan:
            return "ATAN";
        case Op::Actn:
            return "ACTN";
        case Op::Rad:
            return "RAD";
        case Op::Deg:
            return "DEG";
        case Op::Set:
            return "SET";
        default:
            return "?";
    }
}

Op parse_op_token(const std::string& token)
{
    if (token == "+") {
        return Op::Add;
    }
    if (token == "-") {
        return Op::Sub;
    }
    if (token == "*") {
        return Op::Mul;
    }
    if (token == "/") {
        return Op::Div;
    }
    if (token == "%") {
        return Op::Rem;
    }
    if (token == "_") {
        return Op::Neg;
    }
    if (token == "^") {
        return Op::Pow;
    }
    if (token == "SQRT") {
        return Op::Sqrt;
    }
    if (token == "SIN") {
        return Op::Sin;
    }
    if (token == "COS") {
        return Op::Cos;
    }
    if (token == "TAN") {
        return Op::Tan;
    }
    if (token == "CTN") {
        return Op::Ctn;
    }
    if (token == "ASIN") {
        return Op::Asin;
    }
    if (token == "ACOS") {
        return Op::Acos;
    }
    if (token == "ATAN") {
        return Op::Atan;
    }
    if (token == "ACTN") {
        return Op::Actn;
    }
    if (token == "RAD") {
        return Op::Rad;
    }
    if (token == "DEG") {
        return Op::Deg;
    }
    return Op::Invalid;
}

bool parse_fold_op(const std::string& token, Op& op)
{
    if (token.size() != 3 || token.front() != '(' || token.back() != ')') {
        return false;
    }

    op = parse_op_token(token.substr(1, 1));
    return is_binary(op) && op != Op::Set;
}

bool parse_number(const std::string& token, double& value, std::string& error)
{
    if (token.empty()) {
        error = "Bad numeric argument: " + token;
        return false;
    }

    std::size_t index = 0;
    if (token[index] == '+' || token[index] == '-') {
        ++index;
    }

    bool seen_digit = false;
    bool seen_dot = false;
    std::size_t digit_count = 0;

    for (; index < token.size(); ++index) {
        const char ch = token[index];
        if (std::isdigit(static_cast<unsigned char>(ch)) != 0) {
            seen_digit = true;
            ++digit_count;
        } else if (ch == '.' && !seen_dot) {
            seen_dot = true;
        } else {
            error = "Bad numeric argument: " + token;
            return false;
        }
    }

    if (!seen_digit) {
        error = "Bad numeric argument: " + token;
        return false;
    }

    if (digit_count > kMaxDecimalDigits) {
        error = "Too many decimal digits in argument: " + token;
        return false;
    }

    try {
        std::size_t parsed = 0;
        value = std::stod(token, &parsed);
        if (parsed != token.size()) {
            error = "Bad numeric argument: " + token;
            return false;
        }
    } catch (...) {
        error = "Bad numeric argument: " + token;
        return false;
    }

    return true;
}

bool looks_like_number_token(const std::string& token)
{
    if (token.empty()) {
        return false;
    }

    std::size_t index = 0;
    if (token[index] == '+' || token[index] == '-') {
        ++index;
    }

    if (index == token.size()) {
        return false;
    }

    return std::isdigit(static_cast<unsigned char>(token[index])) != 0 || token[index] == '.';
}

bool parse_number_or_report(const std::string& token, double& value)
{
    std::string error;
    if (parse_number(token, value, error)) {
        return true;
    }

    std::cerr << error << std::endl;
    return false;
}

bool near_zero(const double value)
{
    return std::fabs(value) <= kEpsilon;
}

double to_radians(const double value, const bool rad_on)
{
    return rad_on ? value : value * kPi / 180.0;
}

double from_radians(const double value, const bool rad_on)
{
    return rad_on ? value : value * 180.0 / kPi;
}

bool apply_binary(const Op op, const double left, const double right, double& result)
{
    switch (op) {
        case Op::Set:
            result = right;
            return true;
        case Op::Add:
            result = left + right;
            return true;
        case Op::Sub:
            result = left - right;
            return true;
        case Op::Mul:
            result = left * right;
            return true;
        case Op::Div:
            if (near_zero(right)) {
                std::cerr << "Bad right argument for division: " << right << std::endl;
                return false;
            }
            result = left / right;
            return true;
        case Op::Rem:
            if (near_zero(right)) {
                std::cerr << "Bad right argument for remainder: " << right << std::endl;
                return false;
            }
            result = std::fmod(left, right);
            return true;
        case Op::Pow:
            result = std::pow(left, right);
            if (!std::isfinite(result)) {
                std::cerr << "Bad arguments for power: " << left << ", " << right << std::endl;
                return false;
            }
            return true;
        default:
            return false;
    }
}

bool apply_unary(const Op op, const double current, const bool rad_on, double& result)
{
    switch (op) {
        case Op::Neg:
            result = -current;
            return true;
        case Op::Sqrt:
            if (current < 0) {
                std::cerr << "Bad argument for SQRT: " << current << std::endl;
                return false;
            }
            result = std::sqrt(current);
            return true;
        case Op::Sin:
            result = std::sin(to_radians(current, rad_on));
            return true;
        case Op::Cos:
            result = std::cos(to_radians(current, rad_on));
            return true;
        case Op::Tan: {
            const double radians = to_radians(current, rad_on);
            if (near_zero(std::cos(radians))) {
                std::cerr << "Bad argument for TAN: " << current << std::endl;
                return false;
            }
            result = std::tan(radians);
            return true;
        }
        case Op::Ctn: {
            const double radians = to_radians(current, rad_on);
            if (near_zero(std::sin(radians))) {
                std::cerr << "Bad argument for CTN: " << current << std::endl;
                return false;
            }
            result = std::cos(radians) / std::sin(radians);
            return true;
        }
        case Op::Asin:
            if (current < -1.0 - kEpsilon || current > 1.0 + kEpsilon) {
                std::cerr << "Bad argument for ASIN: " << current << std::endl;
                return false;
            }
            result = from_radians(std::asin(std::max(-1.0, std::min(1.0, current))), rad_on);
            return true;
        case Op::Acos:
            if (current < -1.0 - kEpsilon || current > 1.0 + kEpsilon) {
                std::cerr << "Bad argument for ACOS: " << current << std::endl;
                return false;
            }
            result = from_radians(std::acos(std::max(-1.0, std::min(1.0, current))), rad_on);
            return true;
        case Op::Atan:
            result = from_radians(std::atan(current), rad_on);
            return true;
        case Op::Actn:
            result = from_radians(kPi / 2.0 - std::atan(current), rad_on);
            return true;
        default:
            return false;
    }
}

bool apply_fold(
    const double current,
    const Op op,
    const std::vector<std::string>& tokens,
    double& result)
{
    if (tokens.empty()) {
        std::cerr << "No arguments for a fold operation" << std::endl;
        return false;
    }

    double accumulator = current;
    for (const std::string& token : tokens) {
        double argument = 0;
        if (!parse_number_or_report(token, argument)) {
            return false;
        }

        double next = accumulator;
        if (!apply_binary(op, accumulator, argument, next)) {
            return false;
        }
        accumulator = next;
    }

    result = accumulator;
    return true;
}

double process_line_impl(const double current, bool& rad_on, const std::string& line)
{
    const std::string trimmed = trim(line);
    if (trimmed.empty()) {
        return current;
    }

    const std::vector<std::string> tokens = split(trimmed);
    if (tokens.empty()) {
        return current;
    }

    Op fold_op = Op::Invalid;
    if (parse_fold_op(tokens.front(), fold_op)) {
        double result = current;
        if (!apply_fold(
                current,
                fold_op,
                std::vector<std::string>(tokens.begin() + 1, tokens.end()),
                result)) {
            return current;
        }
        return result;
    }

    double literal = 0;
    std::string parse_error;
    if (parse_number(tokens.front(), literal, parse_error)) {
        if (tokens.size() != 1) {
            std::cerr << "Unexpected suffix for a set operation: '" << join_suffix(tokens, 1) << "'" << std::endl;
            return current;
        }
        return literal;
    }
    if (looks_like_number_token(tokens.front())) {
        std::cerr << parse_error << std::endl;
        return current;
    }

    const Op op = parse_op_token(tokens.front());
    if (op == Op::Invalid) {
        std::cerr << "Unknown operation " << line << std::endl;
        return current;
    }

    if (is_mode_switch(op)) {
        if (tokens.size() != 1) {
            std::cerr << "Unexpected suffix for a mode operation: '" << join_suffix(tokens, 1) << "'" << std::endl;
            return current;
        }
        rad_on = (op == Op::Rad);
        return current;
    }

    if (is_unary(op)) {
        if (tokens.size() != 1) {
            std::cerr << "Unexpected suffix for a unary operation: '" << join_suffix(tokens, 1) << "'" << std::endl;
            return current;
        }

        double result = current;
        if (!apply_unary(op, current, rad_on, result)) {
            return current;
        }
        return result;
    }

    if (is_binary(op)) {
        if (tokens.size() < 2) {
            std::cerr << "No argument for a binary operation" << std::endl;
            return current;
        }
        if (tokens.size() > 2) {
            std::cerr << "Unexpected suffix for a binary operation: '" << join_suffix(tokens, 2) << "'" << std::endl;
            return current;
        }

        double argument = 0;
        if (!parse_number_or_report(tokens[1], argument)) {
            return current;
        }

        double result = current;
        if (!apply_binary(op, current, argument, result)) {
            return current;
        }
        return result;
    }

    std::cerr << "Unknown operation " << op_name(op) << std::endl;
    return current;
}

} // namespace

double process_line(const double current, bool& rad_on, const std::string& line)
{
    return process_line_impl(current, rad_on, line);
}

double process_line(const double current, const std::string& line)
{
    thread_local bool rad_on = true;
    return process_line_impl(current, rad_on, line);
}
