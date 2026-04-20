#include <iostream>

#include "CRTP.hpp"
// 奇异递归模板模式

// 普通模板类
// 1. 定义模板
template <typename T>
class Stack {
    T data[100];
    int top = -1;
public:
    void push(T val) { data[++top] = val; }
};

// CRTP模板类
template <typename T>
class Base {
public:
    void interface() {
        // 关键：虽然我现在是Base，但我知道T就是未来的子类
        // 所以我敢把 this 强转成 T*
        static_cast<T*>(this)->implementation();
    }
};

// 这里 Derived 还没定义完，但它的名字已经可以用了
class Derived : public Base<Derived> { 
public:
    void implementation() {
        std::cout << "Derived work!" << std::endl;
    }
};

// CRTP实战例子
template <typename T>
struct NumericTraits {
    // 只要你给我一个 add()，我送你全家桶：
    T& operator++() { static_cast<T*>(this)->add(1); return *static_cast<T*>(this); }
    
    T operator++(int) { 
        T temp = *static_cast<T*>(this); 
        static_cast<T*>(this)->add(1); 
        return temp; 
    }

    T& operator+=(int v) { static_cast<T*>(this)->add(v); return *static_cast<T*>(this); }
    
    // ... 还可以继续塞入 --, -= 等等
};

class Score : public NumericTraits<Score> {
public:
    int val = 0;
    void add(int v) { val += v; } // 子类只写这一个核心逻辑
};

class Counter : public NumericTraits<Counter> {
public:
    int count = 0;
    void add(int c) { 
        count += c;
    }
};int main(){
    // 普通模板
    Stack<int> intStack;    // 编译器此时生成一个处理int的类
    Stack<double> strStack; // 编译器此时生成一个处理double的类
    // CRTP
    Derived d;
    d.implementation();
    // CRTP实战例子
    Counter c;
    ++c; // 重点！Counter 类里并没有定义 operator++，但它可以直接用
    ++c;
    ++c;
    std::cout << "Counter: " << c.count << std::endl; // 输出 1

    Score s;
    ++s; // Score 也可以直接用 ++
    std::cout << "Score: " << s.val << std::endl; // 输出 100
    return 0;
}