#include <iostream>

#include "stats.hpp"

int main() {
    touchA();
    touchB();

    if (g_requests != 2) {
        return 1;
    }

    std::cout << "ok\n";
    return 0;
}
