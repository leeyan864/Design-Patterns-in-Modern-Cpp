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

// 书中讲的Gro0vey没学明白，省略

// 组合构造器
// 假设记录关于一个人的信息Person的成员变量中包含:地址信息和就业信息
// 如果我们想为这两类信息提供单独的建造者，为此将构建一个复合建造者。
class PersonAddressBuilder;
class PersonJobBuilder;
class PersonBuilder;

class Person
{
    // address
    std::string street_address, post_code, city;
    // employment
    std::string company_name, position;
    int annual_income = 0;
    Person() {}

public:
    // 静态工厂方法，开启链式调用
    static PersonBuilder create();
    // 效果展示函数
    std::string to_string() const
    {
        std::ostringstream oss;
        oss << "--- Person Details ---\n"
            << "Address: " << street_address << ", " << post_code << ", " << city << "\n"
            << "Employment: " << company_name << " as a " << position
            << " (Earning " << annual_income << ")\n";
        return oss.str();
    }

    friend class PersonBuilderBase;
    friend class PersonAddressBuilder;
    friend class PersonJobBuilder;
    friend class PersonBuilder;
};

// 这里只是用了person的引用而不存储person
// 拷贝构造函数声明为protected，只有派生类可以使用它
// operator Person() 转换运算符，神技
// lives和works是两个返回构建者的函数：分别完成对地址/工作的构建
class PersonBuilderBase
{
protected:
    Person &person;
    explicit PersonBuilderBase(Person &person) : person{person} {}

public:
    operator Person() { return std::move(person); }
    // builder facets
    PersonAddressBuilder lives() const;
    PersonJobBuilder works() const;
};

// 上一个基类中唯一缺少的是正在构造的实际对象
// 它存储在PersonBuilder的继承者中,这是希望实际使用的类
class PersonBuilder : public PersonBuilderBase
{
    // 实际的对象
    Person p;

public:
    PersonBuilder() : PersonBuilderBase{p} {}
};

class PersonAddressBuilder : public PersonBuilderBase
{
public:
    explicit PersonAddressBuilder(Person &person) : PersonBuilderBase{person} {}
    PersonAddressBuilder &at(std::string street_address)
    {
        person.street_address = street_address;
        return *this;
    }
    PersonAddressBuilder &in(std::string city)
    {
        person.city = city;
        return *this;
    }
    PersonAddressBuilder &with_postcode(std::string post_code)
    {
        person.post_code = post_code;
        return *this;
    }
};

class PersonJobBuilder : public PersonBuilderBase
{
public:
    explicit PersonJobBuilder(Person &person) : PersonBuilderBase{person} {}
    PersonJobBuilder &at(std::string company_name)
    {
        person.company_name = company_name;
        return *this;
    }
    PersonJobBuilder &as_a(std::string position)
    {
        person.position = position;
        return *this;
    }
    PersonJobBuilder &earning(int annual_income)
    {
        person.annual_income = annual_income;
        return *this;
    }
};

PersonBuilder Person::create()
{
    return PersonBuilder{};
}

PersonAddressBuilder PersonBuilderBase::lives() const
{
    return PersonAddressBuilder{person};
}

PersonJobBuilder PersonBuilderBase::works() const
{
    return PersonJobBuilder{person};
}

void example5()
{
    Person p = Person::create()
                   .lives()
                   .at("123 London Road")
                   .with_postcode("SW1 1GB")
                   .in("London")
                   .works()
                   .at("PragmaSoft")
                   .as_a("Consultant")
                   .earning(16);
    std::cout << p.to_string() << std::endl;
}

int main()
{
    example1();
    example2();
    example3();
    example4();
    example5();
    return 0;
}