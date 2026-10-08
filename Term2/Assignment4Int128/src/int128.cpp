#include "int128.hpp"

#include <cctype>
#include <algorithm>
#include <cmath>
#include <ostream>
#include <stdexcept>

namespace {

constexpr uint64_t kAllBitsSet = ~uint64_t{0};

} // namespace

Int128::Int128(uint64_t high, uint64_t low) noexcept
    : high_(high), low_(low) {
}

Int128::Int128(int64_t value) noexcept
    : high_(value < 0 ? kAllBitsSet : 0),
      low_(static_cast<uint64_t>(value)) {
}

Int128::Int128(std::string_view text) {
    if (text.empty()) {
        throw std::invalid_argument("Int128 string is empty");
    }

    bool negative = false;
    std::size_t index = 0;

    if (!text.empty() && (text.front() == '+' || text.front() == '-')) {
        negative = text.front() == '-';
        index = 1;
    }

    if (index == text.size()) {
        throw std::invalid_argument("Int128 string has no digits");
    }

    Int128 magnitude;
    const Int128 ten(10);
    const Int128 limit = negative ? min() : max();
    Int128 limit_div_10;
    Int128 limit_mod_10;
    divmodUnsigned(limit, ten, limit_div_10, limit_mod_10);

    for (; index < text.size(); ++index) {
        const char digit = text[index];
        if (std::isdigit(static_cast<unsigned char>(digit)) == 0) {
            throw std::invalid_argument("Invalid Int128 digit");
        }

        const Int128 digit_value(static_cast<int64_t>(digit - '0'));
        if (compareUnsigned(magnitude, limit_div_10) > 0 ||
            (compareUnsigned(magnitude, limit_div_10) == 0 &&
             compareUnsigned(digit_value, limit_mod_10) > 0)) {
            throw std::out_of_range("Int128 value out of range");
        }

        const Int128 previous = magnitude;
        magnitude = shiftLeftUnsigned(previous, 3);
        magnitude += shiftLeftUnsigned(previous, 1);
        magnitude += digit_value;
    }

    const Int128 raw = negative ? negateRaw(magnitude) : magnitude;
    high_ = raw.high_;
    low_ = raw.low_;
}

Int128::operator int64_t() const noexcept {
    return static_cast<int64_t>(low_);
}

Int128::operator double() const noexcept {
    const bool negative = isNegative();
    const Int128 magnitude = negative ? -(*this) : *this;

    const long double high_part =
        std::ldexp(static_cast<long double>(magnitude.high_), 64);
    const long double low_part = static_cast<long double>(magnitude.low_);
    const long double value = high_part + low_part;

    return negative ? -static_cast<double>(value) : static_cast<double>(value);
}

std::string Int128::str() const {
    if (isZero()) {
        return "0";
    }

    const bool negative = isNegative();
    Int128 magnitude = negative ? -(*this) : *this;
    const Int128 ten(10);
    std::string digits;

    while (!magnitude.isZero()) {
        Int128 quotient;
        Int128 remainder;
        divmodUnsigned(magnitude, ten, quotient, remainder);
        digits.push_back(static_cast<char>('0' + static_cast<int>(remainder.low_)));
        magnitude = quotient;
    }

    if (negative) {
        digits.push_back('-');
    }

    std::reverse(digits.begin(), digits.end());
    return digits;
}

bool Int128::isZero() const noexcept {
    return high_ == 0 && low_ == 0;
}

bool Int128::isNegative() const noexcept {
    return (high_ >> 63U) != 0;
}

Int128 Int128::min() noexcept {
    return fromRaw(uint64_t{1} << 63U, 0);
}

Int128 Int128::max() noexcept {
    return fromRaw((uint64_t{1} << 63U) - 1U, kAllBitsSet);
}

Int128& Int128::operator+=(const Int128& other) noexcept {
    const uint64_t old_low = low_;
    low_ += other.low_;
    high_ += other.high_ + (low_ < old_low ? 1U : 0U);
    return *this;
}

Int128& Int128::operator-=(const Int128& other) noexcept {
    return (*this += negateRaw(other));
}

Int128& Int128::operator*=(const Int128& other) noexcept {
    const bool negative = isNegative() != other.isNegative();
    Int128 left = isNegative() ? -(*this) : *this;
    Int128 right = other.isNegative() ? -other : other;
    Int128 result;

    for (unsigned bit = 0; bit < 128; ++bit) {
        if (getBit(right, bit)) {
            result += shiftLeftUnsigned(left, bit);
        }
    }

    *this = negative ? -result : result;
    return *this;
}

Int128& Int128::operator/=(const Int128& other) {
    if (other.isZero()) {
        throw std::domain_error("division by zero");
    }

    const bool negative = isNegative() != other.isNegative();
    const Int128 left = isNegative() ? -(*this) : *this;
    const Int128 right = other.isNegative() ? -other : other;

    Int128 quotient;
    Int128 remainder;
    divmodUnsigned(left, right, quotient, remainder);

    *this = negative ? -quotient : quotient;
    return *this;
}

Int128 Int128::operator-() const noexcept {
    return negateRaw(*this);
}

bool operator==(const Int128& lhs, const Int128& rhs) noexcept {
    return lhs.high_ == rhs.high_ && lhs.low_ == rhs.low_;
}

bool operator!=(const Int128& lhs, const Int128& rhs) noexcept {
    return !(lhs == rhs);
}

bool operator<(const Int128& lhs, const Int128& rhs) noexcept {
    const int64_t lhs_high = static_cast<int64_t>(lhs.high_);
    const int64_t rhs_high = static_cast<int64_t>(rhs.high_);

    if (lhs_high != rhs_high) {
        return lhs_high < rhs_high;
    }

    return lhs.low_ < rhs.low_;
}

bool operator<=(const Int128& lhs, const Int128& rhs) noexcept {
    return !(rhs < lhs);
}

bool operator>(const Int128& lhs, const Int128& rhs) noexcept {
    return rhs < lhs;
}

bool operator>=(const Int128& lhs, const Int128& rhs) noexcept {
    return !(lhs < rhs);
}

Int128 operator+(Int128 lhs, const Int128& rhs) noexcept {
    lhs += rhs;
    return lhs;
}

Int128 operator-(Int128 lhs, const Int128& rhs) noexcept {
    lhs -= rhs;
    return lhs;
}

Int128 operator*(Int128 lhs, const Int128& rhs) noexcept {
    lhs *= rhs;
    return lhs;
}

Int128 operator/(Int128 lhs, const Int128& rhs) {
    lhs /= rhs;
    return lhs;
}

std::ostream& operator<<(std::ostream& out, const Int128& value) {
    out << value.str();
    return out;
}

Int128 Int128::fromRaw(uint64_t high, uint64_t low) noexcept {
    return Int128(high, low);
}

Int128 Int128::negateRaw(const Int128& value) noexcept {
    const uint64_t low = ~value.low_ + 1U;
    const uint64_t carry = low == 0 ? 1U : 0U;
    const uint64_t high = ~value.high_ + carry;
    return fromRaw(high, low);
}

int Int128::compareUnsigned(const Int128& lhs, const Int128& rhs) noexcept {
    if (lhs.high_ < rhs.high_) {
        return -1;
    }
    if (lhs.high_ > rhs.high_) {
        return 1;
    }
    if (lhs.low_ < rhs.low_) {
        return -1;
    }
    if (lhs.low_ > rhs.low_) {
        return 1;
    }
    return 0;
}

Int128 Int128::shiftLeftUnsigned(const Int128& value, unsigned shift) noexcept {
    if (shift == 0) {
        return value;
    }
    if (shift >= 128) {
        return Int128();
    }
    if (shift >= 64) {
        return fromRaw(value.low_ << (shift - 64U), 0);
    }

    return fromRaw(
        (value.high_ << shift) | (value.low_ >> (64U - shift)),
        value.low_ << shift);
}

Int128 Int128::shiftRightUnsigned(const Int128& value, unsigned shift) noexcept {
    if (shift == 0) {
        return value;
    }
    if (shift >= 128) {
        return Int128();
    }
    if (shift >= 64) {
        return fromRaw(0, value.high_ >> (shift - 64U));
    }

    return fromRaw(
        value.high_ >> shift,
        (value.low_ >> shift) | (value.high_ << (64U - shift)));
}

bool Int128::getBit(const Int128& value, unsigned index) noexcept {
    if (index < 64) {
        return ((value.low_ >> index) & 1U) != 0;
    }
    return ((value.high_ >> (index - 64U)) & 1U) != 0;
}

void Int128::setBit(Int128& value, unsigned index) noexcept {
    if (index < 64) {
        value.low_ |= (uint64_t{1} << index);
        return;
    }

    value.high_ |= (uint64_t{1} << (index - 64U));
}

void Int128::divmodUnsigned(
    const Int128& dividend,
    const Int128& divisor,
    Int128& quotient,
    Int128& remainder) noexcept {
    quotient = Int128();
    remainder = Int128();

    if (divisor.isZero()) {
        return;
    }

    for (int bit = 127; bit >= 0; --bit) {
        remainder = shiftLeftUnsigned(remainder, 1);

        if (getBit(dividend, static_cast<unsigned>(bit))) {
            remainder.low_ |= 1U;
        }

        if (compareUnsigned(remainder, divisor) >= 0) {
            remainder -= divisor;
            setBit(quotient, static_cast<unsigned>(bit));
        }
    }
}
