#pragma once

#include <cstddef>
#include <vector>

class AVLTree
{
public:
    AVLTree() = default;
    AVLTree(const AVLTree& other);
    AVLTree(AVLTree&& other) noexcept;
    AVLTree& operator=(const AVLTree& other);
    AVLTree& operator=(AVLTree&& other) noexcept;

    bool contains(int value) const;
    bool insert(int value);
    bool remove(int value);

    std::size_t size() const noexcept;
    bool empty() const noexcept;

    std::vector<int> values() const;

    ~AVLTree();

    friend void swap(AVLTree& lhs, AVLTree& rhs) noexcept;

private:
    struct Node
    {
        explicit Node(int newValue)
            : value(newValue)
        {
        }

        int value;
        int height = 0;
        Node* left = nullptr;
        Node* right = nullptr;
    };

    static int nodeHeight(const Node* node);
    static void updateHeight(Node* node);
    static int balanceFactor(const Node* node);

    static Node* rotateLeft(Node* node);
    static Node* rotateRight(Node* node);
    static Node* rebalance(Node* node);

    static Node* clone(const Node* node);
    static void destroy(Node* node);
    static void inOrder(const Node* node, std::vector<int>& result);

    static Node* insert(Node* node, int value, bool& inserted);
    static Node* remove(Node* node, int value, bool& removed);
    static Node* detachMin(Node* node, Node*& minNode);

    Node* root_ = nullptr;
    std::size_t size_ = 0;
};
