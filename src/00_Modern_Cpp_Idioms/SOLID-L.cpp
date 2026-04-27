// Single 单一职责原则
// Open-Closed 开闭原则
// Liskov 里氏替换原则
// Interface 接口隔离原则
// Dependency 依赖倒置原则
#include <iostream>
#include <string>
#include <vector>

using std::string;

// ===============接口隔离原则==============
// 将复杂接口的部分分隔成单独的接口，以避免迫使实现者实现他们并不真正需要的功能

// 假设你有个打印机：可以打印、扫描、传真
struct MyFavouritePrinter /* : IMachine */
{
  void print(vector<Document *> docs) override;
  void fax(vector<Document *> docs) override;
  void scan(vector<Document *> docs) override;
};

// 你决定定义一个需要由所有计划制造多功能打印机的人实现的接口
// 有一个问题。这个接口的一些实现者可能不需要扫描或传真，只需要打印
// 然而，你强迫他们实现这些额外的功能
struct IMachine {
  virtual void print(vector<Document *> docs) = 0;
  virtual void fax(vector<Document *> docs) = 0;
  virtual void scan(vector<Document *> docs) = 0;
};

// 建议是将接口分开，以便于实现者可以根据他们的需求进行挑选和选择
struct IPrinter {
  virtual void print(vector<Document *> docs) = 0;
};

struct IScanner {
  virtual void scan(vector<Document *> docs) = 0;
};

int main() { return 0; }