// Author: Egor Kadomtsev
// Group: 25.Б81-мм
// Contact: st116930@student.spbu.ru

#include "rpn.h"
#include <iostream>
#include <string>

int main() {
    std::cout << "Enter RPN expression (or 'exit' to quit):" << std::endl;
    std::string line;
    while (std::getline(std::cin, line)) {
        if (line == "exit") break;
        if (line.empty()) continue;
        try {
            double result = rpncalc::evaluate_rpn(line);
            std::cout << "Result: " << result << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }
    }
    return 0;
}
