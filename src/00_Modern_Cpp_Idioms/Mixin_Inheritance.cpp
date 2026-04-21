#include <iostream>
#include <cstdint>
// Mixin继承、
/*
传统的继承是“我是什么”（例如：Dog 是 Animal）。
Mixin 继承则是“我能做什么”（例如：这个类可以“混合”进“打招呼”的能力）
*/
// c++中，类可以继承它的模板参数
template <typename T> struct Mixin : T {
  // ...
};
/*它把我们熟悉的继承关系给“倒过来”了。
1. 传统继承：你是谁
在传统的 OOP（面向对象）里，你要先定义好一个家谱
你定义了 `Bread`（面包）
你想做一个肉松面包，你得写：`class MeatFlossBread : public Bread`
你想做一个火腿肉松面包，你得再写：`class HamMeatFlossBread : public
MeatFlossBread` 缺点:如果你想要 10
种不同的配料组合，你就得写几十个类，代码会变得臃肿不堪。

2. Mixin 继承：给任何东西“加料”
Mixin的逻辑是:我不关心你是谁，我只负责给你加个功能。**
它的写法是：
*/
template <typename T>
class WithHam : public T { // “继承自T”是关键
public:
  void eatHam() { std::cout << "吃到火腿了！"; }
};
/*
这里的`T`是一个占位符。这意味着：
如果`T`是面包，它就变成了*火腿面包*
如果`T`是面条，它就变成了*火腿面条*
如果`T`是石头，它就变成了*带火腿的石头*

3. 为什么Dmitri说这在 C++20很厉害？
在C++20之前，这种写法很难受，因为你不知道 `T` 到底是个啥，万一
`T`没有构造函数怎么办？ 在第一章里，Dmitri 强调了 Mixin 的两个核心威力：
1.  “套娃”式组合
    你可以像乐高一样拼装对象：
    `WithEggs<WithHam<Bread>> myBreakfast;`
    这一行代码就通过继承链，瞬间生成了一个既有蛋又有火腿的面包类，而不需要你手动去写一个
`HamAndEggBread` 类。
2.  属性注入:
    如果你有一个普通的类
`Square`（正方形），它只有边长。你想让它变彩色，你不需要修改`Square`的源码，直接写：
    `Colored<Square> redSquare;`
    这个 `redSquare` 现在既有 `Square` 的边长属性，又有 `Colored` 的颜色属性了。

总结:Mixin就是一个“复写纸”模板,当你把它盖在任何一个类`T`上面时，它就会产生一个新的类，这个新类完整保留了原有的能力，并多出了模板里定义的超能力

它的核心公式是：新类 = 插件<原类>
以前`class A : public B`（A 是固定的），而 Mixin 是 `class Mixin : public
T`（基类`T`是变动的）。
*/
// 实战例子，假设你在开发一个图形编辑器。你已经有了一个基础类
// Square，现在你想要灵活地给它增加“颜色”和“透明度”的功能。
struct Square {
  float side;
  explicit Square(float s) : side(s) {}
  std::string str() const {
    return "A square with side " + std::to_string(side);
  }
};

// 插件1：增加颜色功能
template <typename T> struct ColoredShape : T {
  std::string color;

  // 完美转发构造函数：把参数传给底层的 T
  template <typename... Args>
  ColoredShape(std::string c, Args &&...args)
      : T(std::forward<Args>(args)...), color(c) {}

  std::string str() const { return T::str() + " has the color " + color; }
};

// 插件2：增加透明度功能
template <typename T> struct TransparentShape : T {
  uint8_t transparency;

  template <typename... Args>
  TransparentShape(uint8_t t, Args &&...args)
      : T(std::forward<Args>(args)...), transparency(t) {}

  std::string str() const {
    return T::str() + " has " + std::to_string(transparency) + "% transparency";
  }
};
int main() {
  // 组合出一个“红色的正方形”
  ColoredShape<Square> red_square{"red", 5.0f};
  std::cout << red_square.str() << std::endl;

  // 组合出一个“透明度为50%的蓝色的正方形” (三层嵌套！)
  TransparentShape<ColoredShape<Square>> dynamic_shape{50, "blue", 10.0f};
  std::cout << dynamic_shape.str() << std::endl;
  return 0;
}