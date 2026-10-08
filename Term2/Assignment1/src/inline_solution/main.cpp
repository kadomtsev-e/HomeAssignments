#include <iostream>

#include "stats.hpp"

int main() {
    touchA();
    touchB();

    std::cout << g_name << ": requests=" << g_requests << '\n';
    return 0;
}
