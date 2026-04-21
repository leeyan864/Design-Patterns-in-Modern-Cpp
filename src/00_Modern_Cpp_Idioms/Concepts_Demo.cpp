#include <iostream>
#include <string_view>

// 旧风格的静态多态
// 假设构建一个警报系统
template <typename TImpl> class Notifer {
public:
  void AlertSMS(std::string_view msg) { impl().SendAlertSMS(msg); }
  void AlertEmail(std::string_view msg) { impl().SendAlertEmail(msg); }

private:
  TImpl &impl() { return static_cast<TImpl &>(*this); }
  friend TImpl;
};
// 上面这种写法即使没有明确指定TImpl必须继承自Notifer编译器也会检查这两个方法是否存在
// 基于此，我们可以定义一种在所有channel上发送警报的能力
template <typename TImpl>
void AlertAllChannels(Notifer<TImpl> &notifer, std::string_view msg) {
  notifer.AlertEmail(msg);
  notifer.AlertSMS(msg);
}
// 构造一个Notifer的实现，但是上面也不做
struct TestNotifer : public Notifer<TestNotifer> {
  void SendAlertSMS(std::string_view msg) {}
  void SendAlertEmail(std::string_view msg) {}
};

int main() {
  TestNotifer tn;
  AlertAllChannels(tn, "testing!");
  return 0;
}