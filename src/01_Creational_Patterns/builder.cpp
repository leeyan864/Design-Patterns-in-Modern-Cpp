// 建造者模式涉及到复杂对象的创建，也就是难以通过调用单行构造函数来创建的对象
// 这些类型的对象本身可能由其他对象组成，并可能涉及不太明显的逻辑，需要专门设计单独的组件来创建
#include <iostream>
#include <string>
#include <vector>
#include <sstream>

// 假设我们正在构建一个呈现web页面的组件
// 先输出一个无序列表，其中有两个item，包括两个单词
void example1()
{
    std::string words[] = {"hello", "world"};
    std::ostringstream oss;
    oss << "<ul>";
    for (auto w : words)
        oss << " <li>" << w << "</li>";
    oss << "</ul>";
    std::cout << oss.str() << std::endl;
}

// 上面的面向过程不灵活
// 因此可以用oop的方式来定义一个HtmlElement类
struct HtmlElement
{
    std::string name;
    std::string text;
    std::vector<HtmlElement> elements;
    HtmlElement() {}
    HtmlElement(const std::string &name, const std::string &text) : name{name}, text{text} {}
    std::string str(int indent = 0) const
    {
        std::ostringstream oss;
        int indent_size = 1;
        std::string i(indent_size * indent, ' ');
        oss << i << "<" << name << ">" << std::endl;
        if (text.size() > 0)
        {
            oss << std::string(indent_size * (indent + 1), ' ') << text << std::endl;
        }
        for (const auto &e : elements)
        {
            oss << e.str(indent + 1);
        }
        oss << i << "</" << name << ">" << std::endl;
        return oss.str();
    }
};

void example2()
{
    std::string words[] = {"hello", "world"};
    HtmlElement list{"ul", ""};
    for (auto w : words)
        list.elements.emplace_back("li", w);
    std::cout << list.str() << std::endl;
}

// example2做的很好，但是构建每个Element的时候不是很方便
// 还可以通过构建者模式来改进它
struct HtmlBuilder
{
    HtmlElement root;

    HtmlBuilder(std::string root_name) { root.name = root_name; }

    void add_child(std::string child_name, std::string child_text)
    {
        HtmlElement e{child_name, child_text};
        root.elements.emplace_back(e);
    }

    std::string str() { return root.str(); }
};

void example3()
{
    HtmlBuilder builder{"ul"};
    builder.add_child("li", "hello");
    builder.add_child("li", "world");
    std::cout << builder.str() << std::endl;
}

// example3已经做的很好了，但还可以配合链式调用使用
struct HtmlBuilderFLuent
{
    HtmlElement root;

    HtmlBuilderFLuent(std::string root_name) { root.name = root_name; }

    HtmlBuilderFLuent &add_child(std::string child_name, std::string child_text)
    {
        HtmlElement e{child_name, child_text};
        root.elements.emplace_back(e);
        return *this;
    }

    std::string str() { return root.str(); }
};

void example4()
{
    HtmlBuilderFLuent builder{"ul"};
    builder.add_child("li", "hello")
        .add_child("li", "world");
    std::cout << builder.str() << std::endl;
}

int main()
{
    example1();
    example2();
    example3();
    return 0;
}