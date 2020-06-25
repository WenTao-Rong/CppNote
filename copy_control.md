### c++ primer学习笔记系列--拷贝控制

拷贝控制操作：**拷贝构造函数、拷贝赋值运算符、移动构造函数、移动赋值运算符、析构函数**。如果一个类没有对定义所有这些拷贝控制成员，编译器将自动为它定义缺失的操作（移动构造缺失时不一定会定义）

学习到以下内容：

- 编译器对这些操作默认定义是怎样的？

- 什么时候会调用这些操作
- 怎样定义这些拷贝控制操作
- 怎样写行为像指针的类和行为像值的类
- 怎么写一个动态内存类，以vector<string> 为例
- 利用移动操作和右值引用来减少不必要的拷贝

代码：https://github.com/WenTao-Rong/CppNote/tree/master/copy_control

#### 拷贝构造函数

##### 合成拷贝构造函数

合成拷贝构造函数（合成指编译器生成）及其拷贝方式：

对类类型的成员，使用其拷贝构造函数来拷贝，内置类型的成员则直接拷贝；数组会逐一直接拷贝（内置类型成员）或调用拷贝构造函数（类类型成员）

```c++
class Sale_data
{
    public:
    Sale_data(string book,intunits,doublereven)
        :bookNo(book),units_sold(units),revenue(reven){};
    Sale_data(const Sale_data & org):
    bookNo(org.bookNo),units_sold(org.units_sold),revenue(org.revenue){}//等价合成拷贝构造函数，注释后效果一样
    Sale_data & operator =(const Sale_data &rhs)
    {
        bookNo=rhs.bookNo;
        units_sold=rhs.units_sold;
        revenue=rhs.revenue;
        return *this;
    }//等价合成拷贝赋值运算符，注释后效果一样
    ~Sale_data()
    {
        cout<<"Sale_data deconstruct"<<endl;
    }
    void print()
    {
        cout<<"bookno: "<<bookNo<<" unit_sold: "<<units_sold<<" revenue: "<<revenue<<endl;
    };

    private:
    string bookNo;
    int units_sold=0;
    double revenue=0.0;
};
int main()
{
    Sale_data s1("bookk",5,9.34);
    Sale_data s2=s1;
    s2.print();
    Sale_data s3(s1);
    s3.print();
}

```

##### 拷贝初始化

拷贝初始化需要用拷贝构造函数或移动构造来完成

拷贝初始化：使用等号初始化一个变量

直接初始化：不使用等号初始化

```c++
std::string s(10,'.');//直接初始化
std::string s2(s);//直接初始化
std::string s3="string";//拷贝初始化
std::string s4=string(10,'9');//拷贝初始化
std::string s5=s4;//拷贝初始化
```

拷贝初始化不仅在用=定义变量时发生，在以下情况也会发生：

- 传递对象给非引用类型的形参
- 返回一个非引用的类型的对象
- 用花括号初始化数组或聚合类的成员

##### 拷贝构造函数的形参

必须是引用类型参数，否则调用会无限循环



#### 拷贝赋值运算符

合成拷贝赋值运算符见上Sale_data类,注意返回值是引用类型



#### 析构函数

析构函数不接受参数，因此不能被重载

##### 析构函数的工作

先执行函数体，在销毁成员，成员按初始化的逆序销毁。

销毁类成员需要执行成员自身的析构函数，内置类型没有析构函数，因此销毁内置成员什么都不用做

##### 什么时候调用析构函数 

对象被销毁，就会自动调用析构函数：

- 变量离开作用域被销毁
- 当对象被销毁时，其成员被销毁
- 容器被销毁时，其元素被销毁
- 动态分配的对象，使用delete销毁
- 临时对象，当创建完它的完整表达式结束时被销毁

当一个对象的指针或引用离开作用域，析构函数不会执行

```c++
int main()
{
Sale_data * p=new Sale_data("book1",23,4.3);
auto p2=make_shared<Sale_data>("book2",45,9.3);
Sale_data item(*p);
vector<Sale_data> vec;
vec.push_back(*p2);
delete p;
}
//析构函数将执行4次
```

##### 合成析构函数

如果未定义类的析构函数，编译器会定义一个合成析构函数，它的函数体是空的，但析构函数体执行完后，成员会自动被销毁，比如Sale_data的string 成员的析构函数就被调用，但动态内存需要调用delete或者借助智能指针



#### 三/五法则

需要析构函数的类也需要拷贝构造和拷贝赋值操作，比如没有定义拷贝构造和拷贝赋值操作，下面程序会出错

拷贝构造和拷贝赋值共存

5个拷贝控制成员看做一个整体，如果一个类定义任意一个拷贝操作，它应该定义所有5个操作

```c++
class HasPtr
{
    public:
    HasPtr(const std::string  & s=std::string()):ps(new std::string(s)),i(0){}
    ~HasPtr(){std::cout<<"~HasPtr()"<<std::endl;delete ps;}
    void print(){std::cout<<"*ps"<<*ps<<" i="<<i<<std::endl;};
    private:
    std::string *ps;
    int i;
};
HasPtr f(HasPtr hp)
{
    HasPtr ret=hp;//执行合成拷贝赋值操作，res的ps和hp的ps相同的指针值
    return ret;//hp和ret离开作用域后执行析构函数
}
int main()
{
    HasPtr p("some value");
    f(p);
    HasPtr q(p);
    q.print();

}
```

#### 使用=default 和=delete

使用=default(只能对默认构造函数和拷贝控制成员使用)， 在类内修饰成员的声明时，合成的函数为内联函数，在类外修饰，不是内联函数

=delete 用来阻止拷贝 (对任何函数也可以使用，析构函数不可以)，必须出现在函数第一次声明的地方



#### 拷贝控制和资源管理

##### 行为像值的类

每个对象都有一个拷贝

```c++
class HasPtr
{
    public:
    HasPtr(const std::string  & s=std::string()):ps(new std::string(s)),i(0){}
    ~HasPtr(){delete ps;}
    HasPtr(const HasPtr& p):ps(new string(*p.ps)),i(p.i){};
    HasPtr& operator =(const HasPtr& rhs)
    {
        string*  newp=new string(*rhs.ps);
        delete ps;
        ps=newp;
        i=rhs.i;
        return *this;
    }
    void print(){std::cout<<"*ps= "<<*ps<<" i="<<i<<std::endl;};

    private:
    std::string *ps;
    int i;
};

```

类值拷贝赋值运算符

要防范自我赋值的情况，编写一个拷贝赋值运算符的模式是先将右侧运算符对象拷贝到一个局部临时对象中，然后销毁左侧运算对象的现有成员，最后将临时对象拷贝到左侧运算符对象的成员里

```c++
HasPtr& operator =(const HasPtr& rhs)
{
    delete ps;
    ps=new string(*rhs.ps);
    ps.i=rhs.i;
    return *this;
}//错误写法
```

##### 行为像指针的类

使用引用计数

```C++
class HasPtr
{
    public:
    HasPtr(const std::string  & s=std::string()):ps(new std::string(s)),i(0),use(new size_t(1)){};
    ~HasPtr(){
        if(--*use==0) {delete ps;delete use;}
    }
    HasPtr(const HasPtr& p):ps(p.ps),i(p.i){++*use;}
    HasPtr& operator =(const HasPtr& rhs)
    {
        ++*rhs.use;//先增加右操作数的引用个数，自赋值安全
        if(--*use==0) {delete ps;delete use;}
        ps=rhs.ps;
        i=rhs.i;
        use=rhs.use;
        return *this;
    }
    void print(){std::cout<<"*ps= "<<*ps<<" i="<<i<<std::endl;}
    private:
    std::string *ps;
    int i;
    size_t *use;
};

```



#### 交换操作

swap 不是必要的，但对于分配了资源的类，定义swap可能是重要的优化手段

如果一个类没有定义swap，使用到swap的算法将使用标准库定义的swap,例如交换HasPtr对象（HasPtr是一个行为像值的类）的代码如下

```c++
HasPtr temp=v1;
v1=v2;
v2=temp;//这样产生不必要的内存分配
```

在HasPtr中编写自己的swap函数并且在赋值运算中使用swap

```c++
class HasPtr
{
    public:
    HasPtr(const std::string  & s=std::string()):ps(new std::string(s)),i(0){}
    ~HasPtr(){delete ps;}
    HasPtr(const HasPtr& p):ps(new std::string(*p.ps)),i(p.i){}
    HasPtr& operator =(HasPtr rhs)
    {
        swap(*this,rhs);
        return *this;
    }//参数rhs不是引用，离开作用域后，rhs(this原来的内容)被析构了，this指向新创建的副本内容，能处理自赋值
    friend void swap(HasPtr & lhs,HasPtr &rhs);//声明为友员函数，是为了能访问私有成员
    void print(){std::cout<<"*ps= "<<*ps<<" i="<<i<<std::endl;}

    private:
    std::string *ps;
    int i;
};
inline void swap(HasPtr & lhs,HasPtr &rhs)
{
    using std::swap;//使用std的swap
    swap(lhs.ps,rhs.ps);
    swap(lhs.i,rhs.i);
};

```



#### 动态内存管理类

实现一个简化版的vector类，而且只用于string

在重分配内存函数reallocate中，为了避免不必要的拷贝且旧空间的数据不再使用，调用move得到右值引用从而使用string的移动构造函数，而已标准库保证‘“移后源”string保持一个有效的，可析构的状态

```c++
#include<memory>
#include<iostream>
#include<utility>
#include<string>
#include<cstddef>
class StrVec
{
    public:
    StrVec():elements(nullptr),cap(nullptr),first_free(nullptr){};
    ~StrVec(){
        free();
    }
    StrVec(const StrVec& s)
    {       
        auto data=alloc_n_copy(s.begin(),s.end());
        elements=data.first;
        first_free=cap=data.second;

    }
    StrVec& operator =(const StrVec &s)
    {
        auto data=alloc_n_copy(s.begin(),s.end());
        free();
        elements=data.first;
        first_free=cap=data.second;
        return *this;

    }
    void push_back(const std::string & s)
    {
        check_alloc();
        alloc.construct(first_free++,s);
    }
    size_t size()const {return first_free-elements;}
    size_t capacity() const {return cap-elements;};
    std::string* begin() const {return elements;}
    std::string* end() const {return first_free;}

    private:
    std::pair<std::string* ,std::string*>  alloc_n_copy(const std::string * begin,const  std::string * end)
    {
        auto data=alloc.allocate(end-begin);
        return {data,uninitialized_copy(begin,end,data)};
        //uninitialized_copy从begin开始到end拷贝元素到data指定的未构造的内存中
    }
    void reallocate()
    {
        int newcapcity=size()?2*size():1;
        std::string * newdata=alloc.allocate(newcapcity);
        //allocate函数，创建未构造的，原始的内存
        std::string* dest=newdata;
        std::string* elem=elements;
        for(int i=0;i<size();i++)
        {
            alloc.construct(dest++,std::move(*elem++));
            //construct函数 在未构造的内存构造对象
        }
        free();
        elements=newdata;
        first_free=dest;
        cap=newdata+newcapcity;
    }
    void check_alloc(){if(size()==capacity()) reallocate();}
    void free()
    {
        if(elements)
        {
            for(auto p=first_free;p!=elements;p--)
                alloc.destroy(p);//destroy函数对p指向的对象执行析构函数
            alloc.deallocate(elements,cap-elements);//deallocate释放从elements开始的cap-elements个对象的内存
        }
    }
    std::string *elements;//指向分配的内存的首元素
    std::string *cap;//指向分配的内存末尾之后的位置
    std::string *first_free;//指向最后一个元素之后的位置
    static std::allocator<std::string> alloc;


};
std::allocator<std::string> StrVec::alloc;
int main()
{
    StrVec p;
    p.push_back("string1");
    std::cout<<p.size()<<std::endl;
    p.push_back("string2");
    p.push_back("string2");
    std::cout<<p.capacity()<<std::endl;
    StrVec p2(p);
    StrVec p3=p2;
    std::cout<<p2.size()<<std::endl;
    std::cout<<p3.size()<<std::endl;

}
```



#### 移动对象

##### 右值引用

右值引用是必须绑定到右值的引用，通过&&获取右值引用，右值引用只能绑定到一个将要销毁的对象

```c++
int i=42;
int &r=i;//正确
int &&rr=i;//错误
int &r2=i*42;//错误
const int &r3=i*42;//正确，可以一个const的引用绑定到一个右值上
int && rr2=i*42;//正确
int &&rr1=42;///正确
int &&rr3=rr1;//错误
```

返回左值引用的函数，连同赋值、下标、前置递增/递减，都是返回左值的表达式的例子

返回非引用类型的函数，连同算术、关系、后置递增/递减，都生成右值。不能将左值引用绑定到右值上，但可以用const的左值引用或右值引用绑定。

左值持久，右值短暂

##### 标准库move 函数

通过move函数可以获取绑定到左值上的右值引用,但是调用move意味承诺除了赋值或销毁这个左值外，我们不再使用它

move函数可以接受右值和左值

##### 移动构造函数和移动赋值运算符

移动构造函数不仅完成资源移动，而且必须确保移后源对象处于一种状态-销毁它是无害的。也就是说资源完成移动后，原对象不能再指向被移动的资源。可以通过将移后源对象的指针成员置为nullptr实现

另外移动构造函数和移动赋值运算符不应抛出任何异常，所以类的头文件和定义都要指定noexcept，因为移动构造抛出异常时，不安全的，指定noexcept就是告诉编译器我们的移动构造是安全的，否则编译器就必须使用拷贝构造

我们实现StrVec的移动构造函数和移动赋值运算符

```c++
StrVec(StrVec&& s) noexcept:elements(s.elements),cap(s.cap),first_free(s.first_free)
{
    s.elements=s.first_free=s.cap=nullptr;
}
StrVec& operator= (StrVec&& rhs) noexcept
{
    if(this!=&rhs)
    {
        free();
        elements=rhs.elements;
        first_free=rhs.first_free;
        cap=rhs.cap;
        rhs.elements=rhs.first_free=rhs.cap=nullptr;
    }
    return *this;
}

```

##### 合成的移动构造函数和移动赋值运算符

如果一个类定义了自己的拷贝构造函数、拷贝赋值运算符或析构函数，编译器不会合成移动操作。如果没有移动操作，类会使用拷贝操作代替移动操作。

只有一个类没有定义任何自己版本的拷贝控制成员，且每个类非static数据成员都可以移动时，编译器才合成移动构造函数和移动赋值运算符

定义了一个移动构造函数或移动赋值运算符的类一定也定义自己的拷贝操作，否则移动操作默认地被定义为删除的

```c++
#include<memory>
#include<iostream>
#include <string>
struct  X
{
        int i;
        std::string s;
};
struct hasX
{
        X men;
};

int main()
{
        X x;
        x.i=10;
        x.s="string";
        X x2=std::move(x);
        std::cout<<"x: "<<x.s<<" x2:"<<x2.s<<std::endl;//仅用来验证是否调用了移动构造函数
        hasX hx,hx2=std::move(hx);
}

```

##### 什么时候调用移动构造函数和移动赋值运算符

1. 类都有移动构造和拷贝构造函数：

使用函数匹配规则，例如StrVec的拷贝构造可以接受任何转换成StrVec的类型，而移动构造只能接受实参是非static右值

```c++
StrVec v1,v2;
v1=v2;//v2是左值，调用的是拷贝赋值
StrVec getVec(istream &);//假设我们定义了这个函数
v2=getVec(cin)//getVec返回的是非引用类型，是右值，因此调用移动构造函数
```

2. 类有拷贝构造函数，没有移动构造函数：

如果一个类定义了拷贝构造函数，没有移动构造函数，编译器不会合成移动函数的。此时，右值也能通过拷贝构造函数拷贝，因为右值引用可以转换成给一个const左值引用，拷贝函数代替移动构造几乎是安全的。

```c++
#include<iostream>
#include<memory>
class Foo
{
        public:
                Foo()=default;
                Foo(const Foo& ){std::cout<<"Foo copy construct"<<std::endl;}


};
int main()
{
        Foo x;
        Foo y(x);
        Foo z(std::move(x));
}
```

上面程序即使调用move返回一个右值引用但还能使用拷贝拷贝构造

移动赋值运算符和拷贝赋值运算符的调用情况类似

##### 移动迭代器

StrVec的reallocate使用循环方式，我们可以采用另外一种方式：uninitialized_copy+移动迭代器

```c++
for(int i=0;i<size();i++)
{
            alloc.construct(dest++,std::move(*elem++));
            //construct函数 在未构造的内存构造对象
  }//旧版
  uninitialized_copy(make_move_iterator(begin()),make_move_iterator(end()),dest);//新版
        
```

uninitialized_copy对序列中每个元素调用construct，对移动迭代器解引用生成的是一个右值引用，construct将使用移动构造函数来构造元素

##### 成员函数通过右值引用提供移动版本

为StrVec类增加push_back()的移动版本

```c++
void push_back(std::string &&s)
{       
    check_alloc();
    alloc.construct(first_free++,std::move(s));
}
p.push_back("string1");//会调用移动版本
std::stirng s="string2";
p.push_back(s);//调用拷贝版本
```



