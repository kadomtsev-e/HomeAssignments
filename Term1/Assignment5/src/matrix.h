// Author: Egor Kadomtsev
// Group: 25.Б81-мм
// Contact: st116930@student.spbu.ru

#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>
#include <stdexcept>
#include <algorithm>

namespace mtx
{

template <typename T>
class Matrix
{
public:
    // Constructor
    Matrix(size_t rows, size_t cols)
        : _rows(rows), _cols(cols)
    {
        if (_rows == 0 || _cols == 0)
        {
            throw std::invalid_argument("Matrix dimensions must be positive.");
        }
        _data = new T[_rows * _cols]();
    }

    // Copy Constructor
    Matrix(const Matrix& other)
        : _rows(other._rows), _cols(other._cols)
    {
        _data = new T[_rows * _cols];
        std::copy(other._data, other._data + (_rows * _cols), _data);
    }

    // Assignment Operator
    Matrix& operator=(const Matrix& other)
    {
        if (this != &other)
        {
            T* newData = new T[other._rows * other._cols];
            std::copy(other._data, other._data + (other._rows * other._cols), newData);
            delete[] _data;
            _data = newData;
            _rows = other._rows;
            _cols = other._cols;
        }
        return *this;
    }

    // Destructor
    ~Matrix()
    {
        delete[] _data;
    }

    // Accessors
    size_t getRows() const
    {
        return _rows;
    }
    size_t getCols() const
    {
        return _cols;
    }

    T& operator()(size_t r, size_t c)
    {
        if (r >= _rows || c >= _cols)
        {
            throw std::out_of_range("Matrix index out of bounds.");
        }
        return _data[r * _cols + c];
    }

    const T& operator()(size_t r, size_t c) const
    {
        if (r >= _rows || c >= _cols)
        {
            throw std::out_of_range("Matrix index out of bounds.");
        }
        return _data[r * _cols + c];
    }

    // Addition
    Matrix operator+(const Matrix& other) const
    {
        if (_rows != other._rows || _cols != other._cols)
        {
            throw std::invalid_argument("Matrix dimensions mismatch for addition.");
        }
        Matrix result(_rows, _cols);
        for (size_t i = 0; i < _rows * _cols; ++i)
        {
            result._data[i] = _data[i] + other._data[i];
        }
        return result;
    }

    // Scalar Multiplication (matrix * scalar)
    Matrix operator*(const T& scalar) const
    {
        Matrix result(_rows, _cols);
        for (size_t i = 0; i < _rows * _cols; ++i)
        {
            result._data[i] = _data[i] * scalar;
        }
        return result;
    }

    // Matrix Multiplication
    Matrix operator*(const Matrix& other) const
    {
        if (_cols != other._rows)
        {
            throw std::invalid_argument("Matrix dimensions mismatch for multiplication.");
        }
        Matrix result(_rows, other._cols);
        for (size_t i = 0; i < _rows; ++i)
        {
            for (size_t j = 0; j < other._cols; ++j)
            {
                T sum = T(); // Default construct (0 for int/double, 0/1 for Rational)
                for (size_t k = 0; k < _cols; ++k)
                {
                    sum = sum + ((*this)(i, k) * other(k, j));
                }
                result(i, j) = sum;
            }
        }
        return result;
    }

    // Transpose
    Matrix transpose() const
    {
        Matrix result(_cols, _rows);
        for (size_t i = 0; i < _rows; ++i)
        {
            for (size_t j = 0; j < _cols; ++j)
            {
                result(j, i) = (*this)(i, j);
            }
        }
        return result;
    }

    // Ostream
    friend std::ostream& operator<<(std::ostream& os, const Matrix& m)
    {
        for (size_t i = 0; i < m._rows; ++i)
        {
            for (size_t j = 0; j < m._cols; ++j)
            {
                os << m(i, j) << " ";
            }
            os << "\n";
        }
        return os;
    }

    // Equality (useful for tests)
    bool operator==(const Matrix& other) const
    {
        if (_rows != other._rows || _cols != other._cols) return false;
        for (size_t i = 0; i < _rows * _cols; ++i)
        {
            if (_data[i] != other._data[i]) return false;
        }
        return true;
    }

private:
    size_t _rows;
    size_t _cols;
    T* _data;
};

// Scalar Multiplication (scalar * matrix) - free function for left multiplication
template <typename T>
Matrix<T> operator*(const T& scalar, const Matrix<T>& m)
{
    return m * scalar;
}

} // namespace mtx

#endif
