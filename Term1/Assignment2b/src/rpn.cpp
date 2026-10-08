// Author: Egor Kadomtsev
// Group: 25.Б81-мм
// Contact: st116930@student.spbu.ru

#include "rpn.h"
#include "stack.h"
#include <sstream>
#include <stdexcept>

namespace rpncalc {

double evaluate_rpn(const std::string& expression) {
    Stack stack;
    std::stringstream ss(expression);
    std::string token;

    while (ss >> token) {
        if (token == "+" || token == "-" || token == "*" || token == "/") {
            double b = stack.pop();
            double a = stack.pop();
            if (token == "+") stack.push(a + b);
            else if (token == "-") stack.push(a - b);
            else if (token == "*") stack.push(a * b);
            else if (token == "/") {
                if (b == 0) throw std::runtime_error("Division by zero");
                stack.push(a / b);
            }
        } else {
            try {
                stack.push(std::stod(token));
            } catch (...) {
                throw std::invalid_argument("Invalid token: " + token);
            }
        }
    }

    if (stack.size() != 1) {
        throw std::runtime_error("Invalid expression");
    }

    return stack.pop();
}

} // namespace rpncalc
