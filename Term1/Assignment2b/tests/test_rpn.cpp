// Author: Egor Kadomtsev
// Group: 25.Б81-мм
// Contact: st116930@student.spbu.ru

#include <gtest/gtest.h>
#include "../src/rpn.h"
#include "../src/stack.h"

TEST(StackTest, PushPop) {
    Stack s;
    s.push(10);
    s.push(20);
    EXPECT_EQ(s.pop(), 20);
    EXPECT_EQ(s.pop(), 10);
}

TEST(StackTest, Empty) {
    Stack s;
    EXPECT_TRUE(s.isEmpty());
    s.push(1);
    EXPECT_FALSE(s.isEmpty());
}

TEST(RPNTest, SimpleAddition) {
    EXPECT_DOUBLE_EQ(evaluate_rpn("2 3 +"), 5.0);
}

TEST(RPNTest, SimpleSubtraction) {
    EXPECT_DOUBLE_EQ(evaluate_rpn("10 4 -"), 6.0);
}

TEST(RPNTest, ComplexExpression) {
    // (5 + 3) * 2 = 16
    EXPECT_DOUBLE_EQ(evaluate_rpn("5 3 + 2 *"), 16.0);
}

TEST(RPNTest, DivisionByZero) {
    EXPECT_THROW(evaluate_rpn("1 0 /"), std::runtime_error);
}

TEST(RPNTest, InvalidToken) {
    EXPECT_THROW(evaluate_rpn("2 a +"), std::invalid_argument);
}

TEST(RPNTest, NotEnoughOperands) {
    EXPECT_THROW(evaluate_rpn("2 +"), std::underflow_error);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
