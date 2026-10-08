// Author: Egor Kadomtsev
// Group: 25.Б81-мм
// Contact: st116930@student.spbu.ru

#include <iostream>
#include "autobot.h"
#include "decepticon.h"

int main() {
    tf::Weapon cannon(50, 10);
    tf::Autobot prime(10, 500, 300, cannon, 100, true);
    tf::Decepticon megatron(10, 510, 290, cannon, 100, false);

    std::cout << "Optimus Prime level: " << prime.getLevel() << std::endl;
    std::cout << "Megatron level: " << megatron.getLevel() << std::endl;

    if (prime.transform()) {
        std::cout << "Autobots, roll out!" << std::endl;
    }

    return 0;
}
