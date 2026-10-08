// Author: Egor Kadomtsev
// Group: 25.Б81-мм
// Contact: st116930@student.spbu.ru

#include <gtest/gtest.h>
#include "../src/matrix.h"
#include "../src/rational.h"

using namespace mtx;

// ==================== Rational Tests ====================

TEST(RationalTest, BasicOperations)
{
    Rational r1(1, 2);
    Rational r2(1, 4);

    EXPECT_EQ(r1 + r2, Rational(3, 4));
    EXPECT_EQ(r1 - r2, Rational(1, 4));
    EXPECT_EQ(r1 * r2, Rational(1, 8));
    EXPECT_EQ(r1 / r2, Rational(2, 1));
}

TEST(RationalTest, Simplification)
{
    Rational r(2, 4);
    EXPECT_EQ(r.getNum(), 1);
    EXPECT_EQ(r.getDenom(), 2);

    Rational r2(5, -10);
    EXPECT_EQ(r2.getNum(), -1);
    EXPECT_EQ(r2.getDenom(), 2);
}

TEST(RationalTest, DivisionByZero)
{
    EXPECT_THROW(Rational(1, 0), std::invalid_argument);

    Rational r1(1, 2);
    Rational zero(0, 1);
    EXPECT_THROW(r1 / zero, std::invalid_argument);
}

TEST(RationalTest, Comparison)
{
    Rational r1(1, 2);
    Rational r2(1, 3);
    Rational r3(2, 4);

    EXPECT_TRUE(r2 < r1);
    EXPECT_FALSE(r1 < r2);
    EXPECT_EQ(r1, r3);
    EXPECT_NE(r1, r2);
}

TEST(RationalTest, ScalarMultiplication)
{
    Rational r(1, 2);
    EXPECT_EQ(r * 3, Rational(3, 2));
}

// ==================== Matrix<int> Tests ====================

TEST(MatrixIntTest, Constructor)
{
    Matrix<int> m(3, 4);
    EXPECT_EQ(m.getRows(), 3u);
    EXPECT_EQ(m.getCols(), 4u);
}

TEST(MatrixIntTest, InvalidDimensions)
{
    EXPECT_THROW(Matrix<int>(0, 5), std::invalid_argument);
    EXPECT_THROW(Matrix<int>(5, 0), std::invalid_argument);
}

TEST(MatrixIntTest, IndexOperator)
{
    Matrix<int> m(2, 2);
    m(0, 0) = 42;
    EXPECT_EQ(m(0, 0), 42);
}

TEST(MatrixIntTest, IndexOutOfBounds)
{
    Matrix<int> m(2, 2);
    EXPECT_THROW(m(2, 0), std::out_of_range);
    EXPECT_THROW(m(0, 2), std::out_of_range);
}

TEST(MatrixIntTest, CopyConstructor)
{
    Matrix<int> m1(2, 2);
    m1(0, 0) = 1;
    m1(1, 1) = 4;

    Matrix<int> m2(m1);
    EXPECT_EQ(m2(0, 0), 1);
    EXPECT_EQ(m2(1, 1), 4);

    // Verify deep copy
    m1(0, 0) = 99;
    EXPECT_EQ(m2(0, 0), 1); // m2 should be unchanged
}

TEST(MatrixIntTest, AssignmentOperator)
{
    Matrix<int> m1(2, 2);
    m1(0, 0) = 1;
    m1(1, 1) = 4;

    Matrix<int> m2(3, 3); // Different size
    m2 = m1;

    EXPECT_EQ(m2.getRows(), 2u);
    EXPECT_EQ(m2.getCols(), 2u);
    EXPECT_EQ(m2(0, 0), 1);
    EXPECT_EQ(m2(1, 1), 4);

    // Verify deep copy
    m1(0, 0) = 99;
    EXPECT_EQ(m2(0, 0), 1);
}

TEST(MatrixIntTest, Addition)
{
    Matrix<int> m1(2, 2);
    m1(0, 0) = 1;
    m1(0, 1) = 2;
    m1(1, 0) = 3;
    m1(1, 1) = 4;

    Matrix<int> m2(2, 2);
    m2(0, 0) = 1;
    m2(0, 1) = 1;
    m2(1, 0) = 1;
    m2(1, 1) = 1;

    Matrix<int> sum = m1 + m2;
    EXPECT_EQ(sum(0, 0), 2);
    EXPECT_EQ(sum(0, 1), 3);
    EXPECT_EQ(sum(1, 0), 4);
    EXPECT_EQ(sum(1, 1), 5);
}

TEST(MatrixIntTest, AdditionDimensionMismatch)
{
    Matrix<int> m1(2, 2);
    Matrix<int> m2(2, 3);
    EXPECT_THROW(m1 + m2, std::invalid_argument);
}

TEST(MatrixIntTest, ScalarMultiplication)
{
    Matrix<int> m(2, 2);
    m(0, 0) = 1;
    m(0, 1) = 2;
    m(1, 0) = 3;
    m(1, 1) = 4;

    Matrix<int> result = m * 2;
    EXPECT_EQ(result(0, 0), 2);
    EXPECT_EQ(result(1, 1), 8);
}

TEST(MatrixIntTest, ScalarMultiplicationLeft)
{
    Matrix<int> m(2, 2);
    m(0, 0) = 1;
    m(1, 1) = 4;

    Matrix<int> result = 3 * m;
    EXPECT_EQ(result(0, 0), 3);
    EXPECT_EQ(result(1, 1), 12);
}

TEST(MatrixIntTest, Multiplication)
{
    Matrix<int> m1(2, 3);
    m1(0, 0) = 1;
    m1(0, 1) = 2;
    m1(0, 2) = 3;
    m1(1, 0) = 4;
    m1(1, 1) = 5;
    m1(1, 2) = 6;

    Matrix<int> m2(3, 2);
    m2(0, 0) = 7;
    m2(0, 1) = 8;
    m2(1, 0) = 9;
    m2(1, 1) = 1;
    m2(2, 0) = 2;
    m2(2, 1) = 3;

    Matrix<int> res = m1 * m2;
    EXPECT_EQ(res.getRows(), 2u);
    EXPECT_EQ(res.getCols(), 2u);
    EXPECT_EQ(res(0, 0), 31);
    EXPECT_EQ(res(0, 1), 19);
    EXPECT_EQ(res(1, 0), 85);
    EXPECT_EQ(res(1, 1), 55);
}

TEST(MatrixIntTest, MultiplicationDimensionMismatch)
{
    Matrix<int> m1(2, 3);
    Matrix<int> m2(2, 2);
    EXPECT_THROW(m1 * m2, std::invalid_argument);
}

TEST(MatrixIntTest, Transpose)
{
    Matrix<int> m(2, 3);
    m(0, 1) = 5;
    m(1, 2) = 7;

    Matrix<int> t = m.transpose();
    EXPECT_EQ(t.getRows(), 3u);
    EXPECT_EQ(t.getCols(), 2u);
    EXPECT_EQ(t(1, 0), 5);
    EXPECT_EQ(t(2, 1), 7);
}

TEST(MatrixIntTest, Equality)
{
    Matrix<int> m1(2, 2);
    m1(0, 0) = 1;

    Matrix<int> m2(2, 2);
    m2(0, 0) = 1;

    Matrix<int> m3(2, 2);
    m3(0, 0) = 2;

    EXPECT_TRUE(m1 == m2);
    EXPECT_FALSE(m1 == m3);
}

// ==================== Matrix<long> Tests ====================

TEST(MatrixLongTest, BasicOperations)
{
    Matrix<long> m(2, 2);
    m(0, 0) = 1000000000L;
    m(1, 1) = 2000000000L;

    Matrix<long> result = m * 2L;
    EXPECT_EQ(result(0, 0), 2000000000L);
    EXPECT_EQ(result(1, 1), 4000000000L);
}

// ==================== Matrix<float> Tests ====================

TEST(MatrixFloatTest, Addition)
{
    Matrix<float> m1(2, 2);
    m1(0, 0) = 1.5f;
    m1(1, 1) = 2.5f;

    Matrix<float> m2(2, 2);
    m2(0, 0) = 0.5f;
    m2(1, 1) = 0.5f;

    Matrix<float> sum = m1 + m2;
    EXPECT_FLOAT_EQ(sum(0, 0), 2.0f);
    EXPECT_FLOAT_EQ(sum(1, 1), 3.0f);
}

TEST(MatrixFloatTest, ScalarMultiplication)
{
    Matrix<float> m(2, 2);
    m(0, 0) = 1.5f;
    m(0, 1) = 2.0f;

    Matrix<float> res = m * 2.0f;
    EXPECT_FLOAT_EQ(res(0, 0), 3.0f);
    EXPECT_FLOAT_EQ(res(0, 1), 4.0f);
}

// ==================== Matrix<double> Tests ====================

TEST(MatrixDoubleTest, ScalarMult)
{
    Matrix<double> m(2, 2);
    m(0, 0) = 1.5;
    m(0, 1) = 2.0;

    Matrix<double> res = m * 2.0;
    EXPECT_DOUBLE_EQ(res(0, 0), 3.0);
    EXPECT_DOUBLE_EQ(res(0, 1), 4.0);
}

TEST(MatrixDoubleTest, Multiplication)
{
    Matrix<double> m1(2, 2);
    m1(0, 0) = 1.0;
    m1(0, 1) = 2.0;
    m1(1, 0) = 3.0;
    m1(1, 1) = 4.0;

    Matrix<double> m2(2, 2);
    m2(0, 0) = 2.0;
    m2(0, 1) = 0.0;
    m2(1, 0) = 1.0;
    m2(1, 1) = 2.0;

    Matrix<double> res = m1 * m2;
    EXPECT_DOUBLE_EQ(res(0, 0), 4.0);
    EXPECT_DOUBLE_EQ(res(0, 1), 4.0);
    EXPECT_DOUBLE_EQ(res(1, 0), 10.0);
    EXPECT_DOUBLE_EQ(res(1, 1), 8.0);
}

// ==================== Matrix<Rational> Tests ====================

TEST(MatrixRationalTest, Addition)
{
    Matrix<Rational> m1(2, 2);
    m1(0, 0) = Rational(1, 2);
    m1(1, 1) = Rational(1, 3);

    Matrix<Rational> m2(2, 2);
    m2(0, 0) = Rational(1, 2);

    Matrix<Rational> sum = m1 + m2;
    EXPECT_EQ(sum(0, 0), Rational(1, 1));
    EXPECT_EQ(sum(1, 1), Rational(1, 3));
}

TEST(MatrixRationalTest, ScalarMultiplication)
{
    Matrix<Rational> m(2, 2);
    m(0, 0) = Rational(1, 2);
    m(1, 1) = Rational(1, 4);

    Matrix<Rational> scaled = m * Rational(2, 1);
    EXPECT_EQ(scaled(0, 0), Rational(1, 1));
    EXPECT_EQ(scaled(1, 1), Rational(1, 2));
}

TEST(MatrixRationalTest, Multiplication)
{
    Matrix<Rational> m1(2, 2);
    m1(0, 0) = Rational(1, 2);
    m1(0, 1) = Rational(1, 3);
    m1(1, 0) = Rational(1, 4);
    m1(1, 1) = Rational(1, 5);

    Matrix<Rational> identity(2, 2);
    identity(0, 0) = Rational(1, 1);
    identity(1, 1) = Rational(1, 1);

    Matrix<Rational> res = m1 * identity;
    EXPECT_EQ(res(0, 0), Rational(1, 2));
    EXPECT_EQ(res(1, 1), Rational(1, 5));
}

TEST(MatrixRationalTest, Transpose)
{
    Matrix<Rational> m(2, 3);
    m(0, 1) = Rational(2, 3);

    Matrix<Rational> t = m.transpose();
    EXPECT_EQ(t(1, 0), Rational(2, 3));
}

// ==================== Main ====================

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
