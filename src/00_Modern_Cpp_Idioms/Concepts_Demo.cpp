#include <iostream>
#include <string_view>

// 旧风格的静态多态
// 假设构建一个警报系统
template <typename TImpl> class CRTPNotifer {
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
void AlertCRTPAllChannels(CRTPNotifer<TImpl> &notifer, std::string_view msg) {
  notifer.AlertEmail(msg);
  notifer.AlertSMS(msg);
}
// 构造一个Notifer的实现，但是上面也不做
struct TestCRTPNotifer : public CRTPNotifer<TestCRTPNotifer> {
  void SendAlertSMS(std::string_view msg) {}
  void SendAlertEmail(std::string_view msg) {}
};

// 概念与静态多态
template <typename TImpl>
concept IsNotifier = requires(TImpl impl) {
  impl.AlertSMS(std::string_view{});
  impl.AlertEmail(std::string_view{});
};

template <IsNotifier TImpl>
void AlertAllChannels(TImpl &impl, std::string_view msg) {
  impl.AlertSMS(msg);
  impl.AlertEmail(msg);
}

struct TestNotifer {
  void AlertSMS(std::string_view msg) {}
  void AlertEmail(std::string_view msg) {}
};

int main() {
  TestCRTPNotifer tn1;
  // 旧风格的静态多态
  //   AlertCRTPAllChannels(tn1, "Hello CRTP!");
  // 概念与静态多态
  TestNotifer tn2;
  AlertAllChannels(tn2, "Hello C++20 Concepts!");
  return 0;
}