// Single 单一职责原则
// Open-Closed 开闭原则
// Liskov 里氏替换原则
// Interface 接口隔离原则
// Dependency 依赖倒置原则
#include <iostream>
#include <string>
#include <vector>

using std::string;

// ===============依赖倒置原则==============
// 不应该依赖具体的实现，而是实现依赖接口

// GOF中的例子
// 如果你对日志记录感兴趣，你的报告组件不应该依赖于具体的ConsoleLogger，而是可以依赖于ILogger接口
class Reporting {
  ILogger &logger;

public:
  Reporting(const ILogger &logger) : logger{logger} {}
  void prepare_report() {
    logger.log_info("Preparing the report");
    // ...
  }
};

// 问题在于当我们初始化这个类的时候，我们需要显式的调用Reporting{ConsoleLogger{}}或者做类似的操作
// 当reporting同时依赖五个不同的接口会怎么样呢？当然，我们可以编写更多的代码来管理依赖关系
// 处理上述的场景的更加现代化的做法是使用依赖注入：本质上使用一个库(Boost.DI)来满足特殊组件依赖关系
// 看一个设计具有引擎但还需要写入日志的汽车的例子
struct Engine {
  float volume = 5;
  int horse_power = 400;

  friend ostream &operator<<(ostream &os, const Engine &obj) {
    return os << "volume: " << obj.volume << "horse_power: " << obj.horse_power;
  } // thanks, ReSharper!
};

// 现在考虑一件事，我们是否要抽象一个IEngine接口并将其作为参数来传递给小车。
// 我们可能会这么做，也可能不会，这是一个经典的设计决策
// 如果你设想有一个引擎层次结构
// 或者你预见到为了测试目的需要一个NullEngine（参见第19章，空对象模式），那么确实需要抽象出接口

// 同时我们希望通过日志记录小车的行驶速度，这可以用很多种方法来实现（例如通过仪表盘、电子邮件、SMS等）
// 先设计一个接口
struct ILogger {
  virtual ~ILogger() {}
  virtual void Log(const string &s) = 0;
};
// 和某种具体的实现
struct ConsoleLogger : ILogger {
  ConsoleLogger() {}

  void Log(const string &s) override { cout << "LOG: " << s.c_str() << endl; }
};

// 现在我们定义的汽车要依赖engine和logger模块，这里的重点是我们怎么把它俩存储
// 可以使用指针、引用、unique_ptr/shared_ptr或者其他对象
// 将这两个依赖组件定义为构造函数的参数
struct Car {
  unique_ptr<Engine> engine;
  shared_ptr<ILogger> logger;

  Car(unique_ptr<Engine> engine, const shared_ptr<ILogger> &logger)
      : engine{move(engine)}, logger{logger} {
    logger->Log("making a car");
  }

  friend ostream &operator<<(ostream &os, const Car &obj) {
    return os << "car with engine: " << *obj.engine;
  }
};

int main() {
  // 现在，你可能希望在初始化Car时看到对make_unique/make_shared的调用。
  // 但我们不会这么做的。相反，我们将使用Boost.DI。
  // 首先，我们将定义一个绑定，将ILogger绑定到ConsoleLogger
  // 这意味着，只要有人要求一个ILogger，就给他们一个ConsoleLogger
  auto injector = di::make_injector(di::bind<ILogger>().to<ConsoleLogger>());
  auto car = injector.create<shared_ptr<Car>>();
  return 0;
}
