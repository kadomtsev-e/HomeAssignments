#include <iostream>

#include "AVLTree.hpp"

int main()
{
    AVLTree tree;

    for (const int value : {30, 10, 20, 40, 35, 50}) {
        tree.insert(value);
    }

    std::cout << "Tree size: " << tree.size() << '\n';
    std::cout << "Contains 20: " << (tree.contains(20) ? "yes" : "no") << '\n';
    std::cout << "Contains 99: " << (tree.contains(99) ? "yes" : "no") << '\n';

    std::cout << "Values:";
    for (const int value : tree.values()) {
        std::cout << ' ' << value;
    }
    std::cout << '\n';

    tree.remove(30);
    std::cout << "After removing 30:";
    for (const int value : tree.values()) {
        std::cout << ' ' << value;
    }
    std::cout << '\n';

    return 0;
}
