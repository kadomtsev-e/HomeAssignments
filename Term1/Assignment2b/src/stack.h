// Author: Egor Kadomtsev
// Group: 25.Б81-мм
// Contact: st116930@student.spbu.ru

#ifndef STACK_H
#define STACK_H

#include <cstddef>

namespace rpncalc {

class Stack {
public:
    Stack();
    ~Stack();
    
    // Rule of Three: copy operations (using copy-and-swap idiom)
    Stack(const Stack& other);
    Stack& operator=(Stack other);
    
    friend void swap(Stack& a, Stack& b) noexcept;

    void push(double value);
    double pop();
    double peek() const;
    bool isEmpty() const;
    size_t size() const;

private:
    double* data;
    size_t capacity;
    size_t count;
    
    void resize();
};

} // namespace rpncalc

#endif
