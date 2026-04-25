// Single 单一职责原则
// Open-Closed 开闭原则
// Liskov 里氏替换原则
// Interface 接口隔离原则
// Dependency 依赖倒置原则
#include <iostream>
#include <string>
#include <vector>

using std::string;

// ===============单一职责原则==============
// 比如有一个日志类
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

    void add(const string& entry);
};
// 添加一条日记条目是日记实际上需要做的事情，所以将此函数作为 Journal 类的一部分是有意义的.
void Journal::add(const string& entry){
    struct cint count = 1;
    entries.push_back();
}
// 现在你打算把日记保存到文件，就添加save函数.
// 但这有个问题，如果后面我们的设计里其他类也有save函数，后续有个关于save的改动（比如写到服务器而不是磁盘）.
// 就会难以维护，这就是代码里面的坏味道.
void Journal::save(const string& filename){
    ofstream ofs(filename);
    for(auto& s: entries){
        ofs << s << endl;
    }
}


// ================开闭原则================


int main() {
  Journal j1("Dear Diary", 3.14);
  // 编译报错!
  // 阻止了3.14向3转换，虽然在初始化列表用()也会报错，专业一点的写法还是用{}写初始化列表
  // 虽然
  // C++标准规定{}初始化不允许窄化转换，但很多编译器（尤其是GCC和Clang）为了兼容性，默认只给一个
  // Warning（警告）而不是 Error（错误）。 Journal j2{"Dear Diary", 3.14};
  return 0;
}