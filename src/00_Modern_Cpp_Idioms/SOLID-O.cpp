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
// 有一系列的产品。每种产品具有颜色和尺寸定义为
enum class Color { Red, Green, Blue };
enum class Size { Small, Meddium, Large };

struct Product {
  string name;
  Color color;
  Size size;
};

struct ProductFilter {
  typedef std::vector<Product *> Items;
  ProductFilter::Items by_color(Items items, Color color);
  ProductFilter::Items by_size(Items items, Size size);
  ProductFilter::Items by_color_and_size(Items items, Size size, Color color);
};

// 现在要通过颜色来筛选商品
ProductFilter::Items ProductFilter::by_color(Items items, Color color) {
  Items result;
  for (auto &i : items) {
    if (i->color == color) {
      result.push_back(i);
    }
  }
  return result;
}
// 过了一段时间后，需求是通过尺寸来筛选商品
ProductFilter::Items ProductFilter::by_size(Items items, Size size) {
  Items result;
  for (auto &i : items) {
    if (i->size == size) {
      result.push_back(i);
    }
  }
  return result;
}

// 又过了一段时间，需求是通过颜色和尺寸筛选商品
ProductFilter::Items ProductFilter::by_color_and_size(Items items, Size size,
                                                      Color color) {
  Items result;
  for (auto &i : items)
    if (i->size == size && i->color == color)
      result.push_back(i);
  return result;
}

/*
// 可以发现这个非常的不优雅，每次都要加上新的函数，但是逻辑却一致
// 用之前学到的单一职责原则来处理，可以把我们的过滤过程分为两部分:
// 筛选器（接收所有项返回一些项）和规范（用于数据元素的谓词）
 */

// 给规范接口定义
template <typename T> struct Specification {
  virtual bool is_satisfied(T *item) = 0;
};

// 定义一个基于 Specification<T>的过滤方法
template <typename T> struct Filter {
  virtual std::vector<T *> filter(std::vector<T *> items,
                                  Specification<T> &spec) = 0;
};

// 现在我们可以写一个优雅的filter函数
struct BetterFilter : public Filter<Product> {
  virtual std::vector<Product *> filter(std::vector<Product *> items,
                                        Specification<Product> &spec) override {
    std::vector<Product *> result;
    for (auto &p : items) {
      if (spec.is_satisfied(p)) {
        result.push_back(p);
      }
    }

    return result;
  }
};

struct ColorSpecification : Specification<Product> {
  Color color;

  explicit ColorSpecification(const Color color) : color{color} {}

  bool is_satisfied(Product *item) override { return item->color == color; }
};

int main() {
  Product apple{"Apple", Color::Green, Size::Small};
  Product tree{"Tree", Color::Green, Size::Large};
  Product house{"House", Color::Blue, Size::Large};
  ProductFilter::Items all{&apple, &tree, &house};

  BetterFilter bf;
  ColorSpecification green(Color::Green);

  auto green_things = bf.filter(all, green);
  for (auto &x : green_things) {
    std::cout << x->name << " is green" << std::endl;
  }
  return 0;
}