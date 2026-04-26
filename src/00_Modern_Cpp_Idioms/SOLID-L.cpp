// Single 单一职责原则
// Open-Closed 开闭原则
// Liskov 里氏替换原则
// Interface 接口隔离原则
// Dependency 依赖倒置原则
#include <iostream>
#include <string>
#include <vector>

using std::string;

// ===============开闭原则==============
// 接口可以接受类型为Parent的对象，那么它应该同样地可以接受类型为Child的对象，而不会有任何破坏。

// 先看看被破坏的场景
// 矩形有宽度（width）和高度（height），以及计算面积的getters和setters
class Rectangle {
protected:
  int width, height;

public:
  Rectangle(const int width, const int height) : width{width}, height{height} {}

  int get_width() const { return width; }
  virtual void set_width(const int width) { this->width = width; }
  int get_height() const { return height; }
  virtual void set_height(const int height) { this->height = height; }

  int area() const { return width * height; }
};

// 现在有个正方形，重写setters来赋值宽度
// 暂时看不出来问题，但是特殊的时候可能会出错
class Square : public Rectangle {
public:
  Square(int size) : Rectangle(size, size) {}
  void set_width(const int width) override { this->width = height = width; }
  void set_height(const int height) override { this->height = width = height; }
};

// 在这个例子中，如果输入的是矩形Rectangle可以正常work
// 但是如果输入子类Square，会得到错误的结果
// 这就是process违背了里氏替换原则，不能将函数的参数从基类对象替换为子类对象
void process(Rectangle &r) {
  int w = r.get_width();
  r.set_height(10);

  std::cout << "expected area = " << (w * 10) << ", got " << r.area()
            << std::endl;
}


// 解决办法，我认为类型Square甚至不应该存在
// 相反，我们可以创建一个工厂（参见第3章）来创建矩形和正方形
struct RectangleFactory
{
    static Rectangle create_rectangle(int w, int h);
    static Rectangle create_square(int size);
};

int main() {
  Square s{5};
  process(s); // expected area = 50, got 25
  return 0;
}