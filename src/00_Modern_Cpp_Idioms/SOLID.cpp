// Single 单一职责原则
// Open-Closed 开闭原则
// Liskov 里氏替换原则
// Interface 接口隔离原则
// Dependency 依赖倒置原则
#include <iostream>
#include <string>
#include <vector>

using std::string;

struct Journal {
  string title;
  std::vector<string> entries;

  int level;
  explicit Journal(const string &title, const int level)
      : title{title}, level{level} {}
  // explicit Journal(const string& title, const int
  // level):title(title),level(level){} explicit可以防止隐式转换.防止Journal j =
  // "hello";必须显示初始化.
  // 关于这里学到的新知识点，用title{}初始化而不是传统的title().
  // {}可以防止精度丢失，还有一致性可以告诉编译器这是在初始化，而不是函数声明.
};

int main() {
  Journal j1("Dear Diary", 3.14);
  // 编译报错!
  // 阻止了3.14向3转换，虽然在初始化列表用()也会报错，专业一点的写法还是用{}写初始化列表
  // 虽然
  // C++标准规定{}初始化不允许窄化转换，但很多编译器（尤其是GCC和Clang）为了兼容性，默认只给一个
  // Warning（警告）而不是 Error（错误）。 Journal j2{"Dear Diary", 3.14};
  return 0;
}