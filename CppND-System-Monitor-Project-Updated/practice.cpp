#include <iostream>
#include <memory>

class MyClass
{
private:
    int _member;

public:
    MyClass(int val) : _member{val} {}
    void printVal() { std::cout << ", managed object " << this << " with val = " << _member << std::endl; }
    std::unique_ptr<MyClass> f(std::unique_ptr<MyClass> ptr);
};

std::unique_ptr<MyClass> f(std::unique_ptr<MyClass> ptr)
{
    ptr = std::make_unique<MyClass>(5);
    std::cout << "unique_ptr " << &ptr;
    ptr->printVal();
    return std::move(ptr);
}

int main()
{
    std::unique_ptr<MyClass> uniquePtr;// = std::make_unique<MyClass>(23);
    std::cout << "unique_ptr " << &uniquePtr;
    uniquePtr->printVal();

    uniquePtr = f(std::move(uniquePtr));

    if (uniquePtr)
        uniquePtr->printVal();

    return 0;
}