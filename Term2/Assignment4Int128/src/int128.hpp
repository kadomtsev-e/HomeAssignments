#pragma once

#include <cstdint>
#include <iosfwd>
#include <string>
#include <string_view>

class Int128 {
public:
    Int128() noexcept = default;
    Int128(int64_t value) noexcept;
    explicit Int128(std::string_view text);

    explicit operator int64_t() const noexcept;
    explicit operator double() const noexcept;

    [[nodiscard]] std::string str() const;
    [[nodiscard]] bool isZero() const noexcept;
    [[nodiscard]] bool isNegative() const noexcept;

    [[nodiscard]] static Int128 min() noexcept;
    [[nodiscard]] static Int128 max() noexcept;

    Int128& operator+=(const Int128& other) noexcept;
    Int128& operator-=(const Int128& other) noexcept;
    Int128& operator*=(const Int128& other) noexcept;
    Int128& operator/=(const Int128& other);

    [[nodiscard]] Int128 operator-() const noexcept;

    friend bool operator==(const Int128& lhs, const Int128& rhs) noexcept;
    friend bool operator!=(const Int128& lhs, const Int128& rhs) noexcept;
    friend bool operator<(const Int128& lhs, const Int128& rhs) noexcept;
    friend bool operator<=(const Int128& lhs, const Int128& rhs) noexcept;
    friend bool operator>(const Int128& lhs, const Int128& rhs) noexcept;
    friend bool operator>=(const Int128& lhs, const Int128& rhs) noexcept;

    friend Int128 operator+(Int128 lhs, const Int128& rhs) noexcept;
    friend Int128 operator-(Int128 lhs, const Int128& rhs) noexcept;
    friend Int128 operator*(Int128 lhs, const Int128& rhs) noexcept;
    friend Int128 operator/(Int128 lhs, const Int128& rhs);

    friend std::ostream& operator<<(std::ostream& out, const Int128& value);

private:
    uint64_t high_ = 0;
    uint64_t low_ = 0;

    Int128(uint64_t high, uint64_t low) noexcept;

    [[nodiscard]] static Int128 fromRaw(uint64_t high, uint64_t low) noexcept;
    [[nodiscard]] static Int128 negateRaw(const Int128& value) noexcept;
    [[nodiscard]] static int compareUnsigned(const Int128& lhs, const Int128& rhs) noexcept;
    [[nodiscard]] static Int128 shiftLeftUnsigned(const Int128& value, unsigned shift) noexcept;
    [[nodiscard]] static Int128 shiftRightUnsigned(const Int128& value, unsigned shift) noexcept;
    [[nodiscard]] static bool getBit(const Int128& value, unsigned index) noexcept;
    static void setBit(Int128& value, unsigned index) noexcept;
    static void divmodUnsigned(
        const Int128& dividend,
        const Int128& divisor,
        Int128& quotient,
        Int128& remainder) noexcept;
};
