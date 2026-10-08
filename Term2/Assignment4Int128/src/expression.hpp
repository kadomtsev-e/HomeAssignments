#pragma once

#include "int128.hpp"

#include <iosfwd>
#include <map>
#include <memory>
#include <string>
#include <string_view>

using EvaluationContext = std::map<std::string, Int128>;

class Expression {
public:
    virtual ~Expression() = default;

    [[nodiscard]] virtual Int128 eval(const EvaluationContext& values) const = 0;
    [[nodiscard]] virtual Expression* clone() const = 0;
    virtual void print(std::ostream& out) const = 0;
};

class Const final : public Expression {
public:
    explicit Const(Int128 value) noexcept;
    explicit Const(int64_t value) noexcept;

    [[nodiscard]] Int128 eval(const EvaluationContext& values) const override;
    [[nodiscard]] Expression* clone() const override;
    void print(std::ostream& out) const override;

private:
    Int128 value_;
};

class Variable final : public Expression {
public:
    explicit Variable(std::string_view name);

    [[nodiscard]] Int128 eval(const EvaluationContext& values) const override;
    [[nodiscard]] Expression* clone() const override;
    void print(std::ostream& out) const override;

private:
    std::string name_;
};

class Negate final : public Expression {
public:
    explicit Negate(const Expression& operand);

    [[nodiscard]] Int128 eval(const EvaluationContext& values) const override;
    [[nodiscard]] Expression* clone() const override;
    void print(std::ostream& out) const override;

private:
    std::shared_ptr<const Expression> operand_;
};

class Add final : public Expression {
public:
    Add(const Expression& left, const Expression& right);

    [[nodiscard]] Int128 eval(const EvaluationContext& values) const override;
    [[nodiscard]] Expression* clone() const override;
    void print(std::ostream& out) const override;

private:
    std::shared_ptr<const Expression> left_;
    std::shared_ptr<const Expression> right_;
};

class Subtract final : public Expression {
public:
    Subtract(const Expression& left, const Expression& right);

    [[nodiscard]] Int128 eval(const EvaluationContext& values) const override;
    [[nodiscard]] Expression* clone() const override;
    void print(std::ostream& out) const override;

private:
    std::shared_ptr<const Expression> left_;
    std::shared_ptr<const Expression> right_;
};

class Multiply final : public Expression {
public:
    Multiply(const Expression& left, const Expression& right);

    [[nodiscard]] Int128 eval(const EvaluationContext& values) const override;
    [[nodiscard]] Expression* clone() const override;
    void print(std::ostream& out) const override;

private:
    std::shared_ptr<const Expression> left_;
    std::shared_ptr<const Expression> right_;
};

class Divide final : public Expression {
public:
    Divide(const Expression& left, const Expression& right);

    [[nodiscard]] Int128 eval(const EvaluationContext& values) const override;
    [[nodiscard]] Expression* clone() const override;
    void print(std::ostream& out) const override;

private:
    std::shared_ptr<const Expression> left_;
    std::shared_ptr<const Expression> right_;
};

[[nodiscard]] Negate operator-(const Expression& expression);
[[nodiscard]] Add operator+(const Expression& left, const Expression& right);
[[nodiscard]] Subtract operator-(const Expression& left, const Expression& right);
[[nodiscard]] Multiply operator*(const Expression& left, const Expression& right);
[[nodiscard]] Divide operator/(const Expression& left, const Expression& right);

std::ostream& operator<<(std::ostream& out, const Expression& expression);
