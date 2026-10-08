#include "expression.hpp"

#include <ostream>
#include <utility>

namespace {

std::shared_ptr<const Expression> shareClone(const Expression& expression) {
    return std::shared_ptr<const Expression>(expression.clone());
}

void printBinary(
    std::ostream& out,
    const Expression& left,
    const char* op,
    const Expression& right) {
    out << '(' << left << ' ' << op << ' ' << right << ')';
}

} // namespace

Const::Const(Int128 value) noexcept
    : value_(value) {
}

Const::Const(int64_t value) noexcept
    : value_(value) {
}

Int128 Const::eval(const EvaluationContext&) const {
    return value_;
}

Expression* Const::clone() const {
    return new Const(*this);
}

void Const::print(std::ostream& out) const {
    out << value_;
}

Variable::Variable(std::string_view name)
    : name_(name) {
}

Int128 Variable::eval(const EvaluationContext& values) const {
    return values.at(name_);
}

Expression* Variable::clone() const {
    return new Variable(*this);
}

void Variable::print(std::ostream& out) const {
    out << name_;
}

Negate::Negate(const Expression& operand)
    : operand_(shareClone(operand)) {
}

Int128 Negate::eval(const EvaluationContext& values) const {
    return -operand_->eval(values);
}

Expression* Negate::clone() const {
    return new Negate(*this);
}

void Negate::print(std::ostream& out) const {
    out << "(-" << *operand_ << ')';
}

Add::Add(const Expression& left, const Expression& right)
    : left_(shareClone(left)),
      right_(shareClone(right)) {
}

Int128 Add::eval(const EvaluationContext& values) const {
    return left_->eval(values) + right_->eval(values);
}

Expression* Add::clone() const {
    return new Add(*this);
}

void Add::print(std::ostream& out) const {
    printBinary(out, *left_, "+", *right_);
}

Subtract::Subtract(const Expression& left, const Expression& right)
    : left_(shareClone(left)),
      right_(shareClone(right)) {
}

Int128 Subtract::eval(const EvaluationContext& values) const {
    return left_->eval(values) - right_->eval(values);
}

Expression* Subtract::clone() const {
    return new Subtract(*this);
}

void Subtract::print(std::ostream& out) const {
    printBinary(out, *left_, "-", *right_);
}

Multiply::Multiply(const Expression& left, const Expression& right)
    : left_(shareClone(left)),
      right_(shareClone(right)) {
}

Int128 Multiply::eval(const EvaluationContext& values) const {
    return left_->eval(values) * right_->eval(values);
}

Expression* Multiply::clone() const {
    return new Multiply(*this);
}

void Multiply::print(std::ostream& out) const {
    printBinary(out, *left_, "*", *right_);
}

Divide::Divide(const Expression& left, const Expression& right)
    : left_(shareClone(left)),
      right_(shareClone(right)) {
}

Int128 Divide::eval(const EvaluationContext& values) const {
    return left_->eval(values) / right_->eval(values);
}

Expression* Divide::clone() const {
    return new Divide(*this);
}

void Divide::print(std::ostream& out) const {
    printBinary(out, *left_, "/", *right_);
}

Negate operator-(const Expression& expression) {
    return Negate(expression);
}

Add operator+(const Expression& left, const Expression& right) {
    return Add(left, right);
}

Subtract operator-(const Expression& left, const Expression& right) {
    return Subtract(left, right);
}

Multiply operator*(const Expression& left, const Expression& right) {
    return Multiply(left, right);
}

Divide operator/(const Expression& left, const Expression& right) {
    return Divide(left, right);
}

std::ostream& operator<<(std::ostream& out, const Expression& expression) {
    expression.print(out);
    return out;
}
