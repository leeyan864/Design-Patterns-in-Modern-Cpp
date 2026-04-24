// Single 单一职责原则
// Open-Closed 开闭原则
// Liskov 里氏替换原则
// Interface 接口隔离原则
// Dependency 依赖倒置原则



struct Journal{
    string title;
    vector<string> entries;
    explicit Journal(const string& title):title{title}{}
    // explicit可以防止隐式转换.防止Journal j = "hello";必须显示初始化.
    // 关于这里学到的新知识点，用title{}初始化而不是传统的title().
    // {}可以防止精度丢失，还有一致性可以告诉编译器这是在初始化.
}