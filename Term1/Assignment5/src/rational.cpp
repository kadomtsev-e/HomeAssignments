// Author: Egor Kadomtsev
// Group: 25.Б81-мм
// Contact: st116930@student.spbu.ru

#include "rational.h"
#include <numeric>
#include <stdexcept>
#include <cmath>

namespace mtx
{

Rational::Rational(int num, int denom) : _num(num), _denom(denom)
{
    if (_denom == 0)
    {
        throw std::invalid_argument("Denominator cannot be zero.");
    }
    simplify();
}

int Rational::getNum() const
{
    return _num;
}

int Rational::getDenom() const
{
    return _denom;
}

Rational Rational::operator+(const Rational& other) const
{
    return Rational(_num * other._denom + other._num * _denom, _denom * other._denom);
}

Rational Rational::operator-(const Rational& other) const
{
    return Rational(_num * other._denom - other._num * _denom, _denom * other._denom);
}

Rational Rational::operator*(const Rational& other) const
{
    return Rational(_num * other._num, _denom * other._denom);
}

Rational Rational::operator/(const Rational& other) const
{
    if (other._num == 0)
    {
        throw std::invalid_argument("Division by zero.");
    }
    return Rational(_num * other._denom, _denom * other._num);
}

Rational Rational::operator*(int scalar) const
{
    return Rational(_num * scalar, _denom);
}

bool Rational::operator==(const Rational& other) const
{
    return _num == other._num && _denom == other._denom;
}

bool Rational::operator!=(const Rational& other) const
{
    return !(*this == other);
}

bool Rational::operator<(const Rational& other) const
{
    return (_num * other._denom) < (other._num * _denom);
}

void Rational::simplify()
{
    if (_denom < 0)
    {
        _num = -_num;
        _denom = -_denom;
    }
    int common = gcd(std::abs(_num), _denom);
    _num /= common;
    _denom /= common;
}

int Rational::gcd(int a, int b) const
{
    while (b)
    {
        a %= b;
        std::swap(a, b);
    }
    return a;
}

std::ostream& operator<<(std::ostream& os, const Rational& r)
{
    os << r._num;
    if (r._denom != 1)
    {
        os << "/" << r._denom;
    }
    return os;
}

} // namespace mtx
