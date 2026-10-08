#include <cstdlib>
#include <iostream>
#include <random>
#include <set>
#include <sstream>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

#include "AVLTree.hpp"

namespace {

void require(bool condition, const std::string& message)
{
    if (!condition) {
        std::cerr << "Test failed: " << message << '\n';
        std::exit(1);
    }
}

std::string join(const std::vector<int>& values)
{
    std::ostringstream out;
    out << '[';
    for (std::size_t index = 0; index < values.size(); ++index) {
        if (index != 0) {
            out << ", ";
        }
        out << values[index];
    }
    out << ']';
    return out.str();
}

void requireEqual(
    const std::vector<int>& actual,
    const std::vector<int>& expected,
    const std::string& message)
{
    require(
        actual == expected,
        message + ": expected " + join(expected) + ", got " + join(actual));
}

std::vector<int> asVector(const std::set<int>& values)
{
    return std::vector<int>(values.begin(), values.end());
}

} // namespace

int main()
{
    static_assert(
        !std::is_nothrow_copy_assignable<AVLTree>::value,
        "copy assignment should not be noexcept because cloning can allocate");
    static_assert(
        std::is_nothrow_move_assignable<AVLTree>::value,
        "move assignment should remain noexcept");

    AVLTree emptyTree;
    require(emptyTree.empty(), "new tree is empty");
    require(emptyTree.size() == 0, "new tree has zero size");
    requireEqual(emptyTree.values(), {}, "new tree has no values");
    require(!emptyTree.contains(10), "new tree does not contain arbitrary values");
    require(!emptyTree.remove(10), "removing from empty tree returns false");

    AVLTree tree;
    require(tree.insert(30), "first insertion succeeds");
    require(tree.insert(20), "second insertion succeeds");
    require(tree.insert(10), "third insertion succeeds");
    require(tree.insert(40), "fourth insertion succeeds");
    require(tree.insert(50), "fifth insertion succeeds");
    require(tree.insert(35), "sixth insertion succeeds");
    require(!tree.insert(20), "duplicate insertion is rejected");
    require(tree.size() == 6, "size counts unique values only");
    require(!tree.empty(), "tree with values is not empty");
    require(tree.contains(35), "contains finds inserted value");
    require(!tree.contains(99), "contains rejects absent value");
    requireEqual(tree.values(), {10, 20, 30, 35, 40, 50}, "values() returns sorted order");

    AVLTree copied(tree);
    requireEqual(copied.values(), tree.values(), "copy constructor preserves content");
    require(copied.remove(35), "copy can be modified independently");
    require(copied.size() == 5, "copy size changes after removal");
    require(tree.contains(35), "original tree is unchanged after modifying copy");

    AVLTree assigned;
    assigned.insert(-1);
    assigned.insert(100);
    assigned = tree;
    requireEqual(assigned.values(), tree.values(), "copy assignment preserves content");
    require(assigned.insert(60), "assigned tree remains functional");
    require(!tree.contains(60), "copy assignment performs deep copy");

    AVLTree moved(std::move(assigned));
    requireEqual(moved.values(), {10, 20, 30, 35, 40, 50, 60}, "move constructor transfers content");
    require(assigned.empty(), "moved-from tree becomes empty");

    require(tree.remove(10), "removing a leaf succeeds");
    require(tree.remove(40), "removing a node with one child succeeds");
    require(tree.remove(30), "removing a node with two children succeeds");
    require(!tree.remove(30), "removing the same value twice returns false");
    requireEqual(tree.values(), {20, 35, 50}, "tree stays ordered after removals");
    require(tree.size() == 3, "size is updated after removals");

    AVLTree oracleTree;
    std::set<int> oracle;

    for (int value = -200; value <= 200; ++value) {
        const bool inserted = oracleTree.insert(value);
        const bool expectedInserted = oracle.insert(value).second;
        require(inserted == expectedInserted, "bulk insertion matches std::set");
    }

    for (int value = -200; value <= 200; value += 3) {
        require(
            oracleTree.contains(value) == (oracle.count(value) != 0),
            "contains matches std::set before removals");
    }

    requireEqual(oracleTree.values(), asVector(oracle), "bulk insertion keeps full sorted order");

    for (int value = -180; value <= 180; value += 2) {
        const bool removed = oracleTree.remove(value);
        const bool expectedRemoved = oracle.erase(value) != 0;
        require(removed == expectedRemoved, "bulk removal matches std::set");
    }

    requireEqual(oracleTree.values(), asVector(oracle), "bulk removals keep sorted order");
    require(oracleTree.size() == oracle.size(), "size matches std::set after bulk removals");

    for (int value = -200; value <= 200; ++value) {
        require(
            oracleTree.contains(value) == (oracle.count(value) != 0),
            "contains matches std::set after removals");
    }

    while (!oracle.empty()) {
        const int value = *oracle.begin();
        require(oracleTree.remove(value), "remaining values can all be removed");
        oracle.erase(oracle.begin());
    }

    require(oracleTree.empty(), "tree is empty after removing everything");
    require(oracleTree.size() == 0, "size returns to zero after removing everything");
    requireEqual(oracleTree.values(), {}, "values() is empty after removing everything");

    std::mt19937 generator(0xA71U);
    std::uniform_int_distribution<int> values(-500, 500);
    std::bernoulli_distribution insertOperation(0.55);
    AVLTree randomTree;
    std::set<int> randomOracle;

    for (int step = 0; step < 20000; ++step) {
        const int value = values(generator);
        if (insertOperation(generator)) {
            require(
                randomTree.insert(value) == randomOracle.insert(value).second,
                "random insertion matches std::set");
        } else {
            require(
                randomTree.remove(value) == (randomOracle.erase(value) != 0),
                "random removal matches std::set");
        }

        require(randomTree.size() == randomOracle.size(), "randomized size matches std::set");
        requireEqual(randomTree.values(), asVector(randomOracle), "randomized values match std::set");
        require(
            randomTree.contains(value) == (randomOracle.count(value) != 0),
            "randomized contains matches std::set");
    }

    for (int iteration = 0; iteration < 100; ++iteration) {
        AVLTree shortLived(randomTree);
        requireEqual(shortLived.values(), asVector(randomOracle), "repeated copies preserve content");
    }

    std::cout << "All AVL tree tests passed.\n";
    return 0;
}
