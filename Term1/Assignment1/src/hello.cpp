// Author: Egor Kadomtsev
// Group: 25.Б81-мм
// Contact: st116930@student.spbu.ru

#include "hello.h"
#include <iostream>

namespace hello {

void hello_world() {
    std::cout << "Hello, World!" << std::endl;
}

void hello_user(const std::string& name) {
    std::cout << "Hello, " << name << "!" << std::endl;
}

void hello_loop() {
    std::string t;
    while (std::cin >> t) {
        hello_user(t);
    }
}

} // namespace hello