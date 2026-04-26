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

// ===============================================================================

/*
// 可以发现这个非常的不优雅，每次都要加上新的函数，但是逻辑却一致
// 用之前学到的单一职责原则来处理，可以把我们的过滤过程分为两部分:
// 筛选器（接收所有项返回一些项）和规范（用于数据元素的谓词）
 */

// 前向声明
template <typename T> struct AndSpecification;

// 给规范接口定义，要筛选就要继承这个规范接口
template <typename T> struct Specification {
  virtual bool is_satisfied(T *item) const = 0;
  AndSpecification<T> operator&&(const Specification<T> &other);
};

// 定义一个基于规范接口的过滤方法
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

// 实现颜色比较的规范
struct ColorSpecification : public Specification<Product> {
  Color color;

  explicit ColorSpecification(const Color color) : color{color} {}
  bool is_satisfied(Product *item) const override { return item->color == color; }
};

// 实现尺寸比较的规范
struct SizeSpecification : public Specification<Product> {
  Size size;

  explicit SizeSpecification(const Size size) : size{size} {}
  bool is_satisfied(Product *item) const override { return item->size == size; }
};

// 还可以实现更复杂的规范,可以写一个AND规范
template <typename T> struct AndSpecification : public Specification<T> {
  const Specification<T> &first;
  const Specification<T> &second;

  AndSpecification(const Specification<T> &first,const Specification<T> &second)
      : first{first}, second{second} {}
  bool is_satisfied(T *items) const override {
    return first.is_satisfied(items) && second.is_satisfied(items);
  }
};

// 还可以借助运算符重载，创建多个的时候让code更整洁
template<typename T>
AndSpecification<T> Specification<T>::operator&&(const Specification &other) {
  return AndSpecification<T>(*this, other);
}
int main() {
  Product apple{"Apple", Color::Green, Size::Small};
  Product tree{"Tree", Color::Green, Size::Large};
  Product house{"House", Color::Blue, Size::Large};
  ProductFilter::Items all{&apple, &tree, &house};

  BetterFilter bf;
  ColorSpecification green(Color::Green);
  SizeSpecification large(Size::Large);

  // 目前这里也有大量重复code，后续学了其他的也可以优化一下
  auto green_things = bf.filter(all, green);
  for (auto &x : green_things) {
    std::cout << x->name << " is green" << std::endl;
  }

  auto Large_things = bf.filter(all, large);
  for (auto &x : Large_things) {
    std::cout << x->name << " is Large" << std::endl;
  }

  // AND规范
  AndSpecification<Product> green_and_large{large, green};

  auto big_green_things = bf.filter(all, green_and_large);
  for (auto &x : big_green_things) {
    std::cout << x->name << " is large and green" << std::endl;
  }

  // 使用&&运算符
  auto green_and_big = green && large;
  for (auto &x : big_green_things) {
    std::cout << x->name << " is large and green" << std::endl;
  }
  return 0;
}