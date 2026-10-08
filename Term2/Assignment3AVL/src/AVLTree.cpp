#include "AVLTree.hpp"

#include <algorithm>
#include <utility>

AVLTree::AVLTree(const AVLTree& other)
    : root_(clone(other.root_))
    , size_(other.size_)
{
}

AVLTree::AVLTree(AVLTree&& other) noexcept
    : root_(other.root_)
    , size_(other.size_)
{
    other.root_ = nullptr;
    other.size_ = 0;
}

AVLTree& AVLTree::operator=(const AVLTree& other)
{
    if (this != &other) {
        AVLTree copy(other);
        swap(*this, copy);
    }

    return *this;
}

AVLTree& AVLTree::operator=(AVLTree&& other) noexcept
{
    if (this != &other) {
        destroy(root_);
        root_ = other.root_;
        size_ = other.size_;
        other.root_ = nullptr;
        other.size_ = 0;
    }

    return *this;
}

bool AVLTree::contains(int value) const
{
    const Node* current = root_;
    while (current != nullptr) {
        if (value < current->value) {
            current = current->left;
        } else if (value > current->value) {
            current = current->right;
        } else {
            return true;
        }
    }

    return false;
}

bool AVLTree::insert(int value)
{
    bool inserted = false;
    root_ = insert(root_, value, inserted);
    if (inserted) {
        ++size_;
    }

    return inserted;
}

bool AVLTree::remove(int value)
{
    bool removed = false;
    root_ = remove(root_, value, removed);
    if (removed) {
        --size_;
    }

    return removed;
}

std::size_t AVLTree::size() const noexcept
{
    return size_;
}

bool AVLTree::empty() const noexcept
{
    return size_ == 0;
}

std::vector<int> AVLTree::values() const
{
    std::vector<int> result;
    result.reserve(size_);
    inOrder(root_, result);
    return result;
}

AVLTree::~AVLTree()
{
    destroy(root_);
}

void swap(AVLTree& lhs, AVLTree& rhs) noexcept
{
    using std::swap;
    swap(lhs.root_, rhs.root_);
    swap(lhs.size_, rhs.size_);
}

int AVLTree::nodeHeight(const Node* node)
{
    return node == nullptr ? -1 : node->height;
}

void AVLTree::updateHeight(Node* node)
{
    node->height = std::max(nodeHeight(node->left), nodeHeight(node->right)) + 1;
}

int AVLTree::balanceFactor(const Node* node)
{
    return nodeHeight(node->right) - nodeHeight(node->left);
}

AVLTree::Node* AVLTree::rotateLeft(Node* node)
{
    Node* newRoot = node->right;
    node->right = newRoot->left;
    newRoot->left = node;

    updateHeight(node);
    updateHeight(newRoot);
    return newRoot;
}

AVLTree::Node* AVLTree::rotateRight(Node* node)
{
    Node* newRoot = node->left;
    node->left = newRoot->right;
    newRoot->right = node;

    updateHeight(node);
    updateHeight(newRoot);
    return newRoot;
}

AVLTree::Node* AVLTree::rebalance(Node* node)
{
    updateHeight(node);

    if (balanceFactor(node) > 1) {
        if (balanceFactor(node->right) < 0) {
            node->right = rotateRight(node->right);
        }
        return rotateLeft(node);
    }

    if (balanceFactor(node) < -1) {
        if (balanceFactor(node->left) > 0) {
            node->left = rotateLeft(node->left);
        }
        return rotateRight(node);
    }

    return node;
}

AVLTree::Node* AVLTree::clone(const Node* node)
{
    if (node == nullptr) {
        return nullptr;
    }

    Node* copy = new Node(node->value);
    copy->height = node->height;

    try {
        copy->left = clone(node->left);
        copy->right = clone(node->right);
    } catch (...) {
        destroy(copy);
        throw;
    }

    return copy;
}

void AVLTree::destroy(Node* node)
{
    if (node == nullptr) {
        return;
    }

    destroy(node->left);
    destroy(node->right);
    delete node;
}

void AVLTree::inOrder(const Node* node, std::vector<int>& result)
{
    if (node == nullptr) {
        return;
    }

    inOrder(node->left, result);
    result.push_back(node->value);
    inOrder(node->right, result);
}

AVLTree::Node* AVLTree::insert(Node* node, int value, bool& inserted)
{
    if (node == nullptr) {
        inserted = true;
        return new Node(value);
    }

    if (value < node->value) {
        node->left = insert(node->left, value, inserted);
    } else if (value > node->value) {
        node->right = insert(node->right, value, inserted);
    } else {
        return node;
    }

    return rebalance(node);
}

AVLTree::Node* AVLTree::remove(Node* node, int value, bool& removed)
{
    if (node == nullptr) {
        return nullptr;
    }

    if (value < node->value) {
        node->left = remove(node->left, value, removed);
    } else if (value > node->value) {
        node->right = remove(node->right, value, removed);
    } else {
        removed = true;

        if (node->left == nullptr) {
            Node* right = node->right;
            delete node;
            return right;
        }

        if (node->right == nullptr) {
            Node* left = node->left;
            delete node;
            return left;
        }

        Node* successor = nullptr;
        node->right = detachMin(node->right, successor);
        successor->left = node->left;
        successor->right = node->right;

        delete node;
        return rebalance(successor);
    }

    return rebalance(node);
}

AVLTree::Node* AVLTree::detachMin(Node* node, Node*& minNode)
{
    if (node->left == nullptr) {
        minNode = node;
        return node->right;
    }

    node->left = detachMin(node->left, minNode);
    return rebalance(node);
}
