除了静态内存和栈内存，每个程序都拥有一个内存池（堆），程序用堆存储动态分配的对象，即那些程序运行时分配的对象。正确管理动态内存是每个Cpper必须掌握的内容。

学习到以下内容：

- 智能指针shared_ptr,unique_ptr,weak_ptr的使用
- new和delete的使用
- allocator类的使用

代码： 

#### 动态内存和智能指针

##### shared_ptr

允许多个指针指向同一个对象，每一个shared_ptr都有一个关联的计数器（引用计数）,智能指针的使用方式与普通指针类似

| shared_ptr和unique_ptr都支持的操作 |
| ----------------- |
| shared_ptr\<T\> sp  ;unique\<T\> up 空智能指针 |
| p 将p作为一个条件判断，若p指向一个对象，则返回true |
| *p 解引用，获得它指向的对象 |
| p->men 相当于(*p).men |
| p.get() 返回p保存的指针 |
| swap(p,q),p.swap(q) |

| shared_ptr独有的操作                                         |
| ------------------------------------------------------------ |
| make_shared\<T\>(arg)                                        |
| shared_ptr\<T\> p(q) p是shared_ptr q的拷贝，会递增q中的引用计数 |
| p=q  递增q的引用计数，递减p的引用计数                        |
| p.unique() 判断p.use_count()==1                              |
| p.use_count()                                                |

1. 最安全的分配和使用动态内存的方法是调用make_shared函数

2. 拷贝一个shared_ptr，计数器会递增，比如用一个shared_ptr初始化另外一个shared_ptr，或者将它作为一个参数传递到函数以及作为函数的返回值

3. 当shared_ptr赋予新值或者局部的shared_ptr离开作用域被销毁时，计数器会递减

4. 当指向对象的最后一个shared_ptr被销毁时，shared_ptr类会通过析构函数机或者自己定义的删除器自动销毁此对象。

```c++
class Foo
{
    public:
    Foo()=default;
    ~Foo(){cout<<"~Foo()"<<endl;}
};
shared_ptr<Foo> factory()
{
    return make_shared<Foo>();
};
int main()
{
    shared_ptr<string> p1(new string());
    shared_ptr<list<int>>p2;
    if(p1) *p1="hi";
    shared_ptr<int> p3=make_shared<int>(42);
    shared_ptr<string> p4=make_shared<string>(10,'9');
    shared_ptr<int> p5=make_shared<int>();//不传递任何参数，对象进行值初始化
    auto p6=make_shared<vector<string>>();
    auto q(p3);
    cout<<"q.use_count"<<q.use_count()<<endl;
    auto r=make_shared<int>(50);
    r=q;
    shared_ptr<Foo> foo=factory();
    cout<<"q.use_count"<<q.use_count()<<endl;
}
```



#### 各种初始化总结

当对象创建时得到特定的值就说这个对象被初始化，初始化不是赋值

初始化方式：使用{}列表初始化，默认初始化（没有给定初值），拷贝初始化（使用等于），值初化（采用小括号的方式进行变量初始化，没提供初始值），直接初始化（小括号的方式进行变量初始化，提供初始值）

```c++
string* a=new string[10];//默认初始化
int b;//默认初始化

string* c=new string[10]{"1","2","3"};//列表初始化，列表初始化个数小于声明的大小时，数组中其他的值会调用string的默认初始化
string d="string"; //拷贝初始化
int e=new int();//值初始化
string f=new string[10]();//值初始化
vector<string> d(10);//值初始化，会调用string的默认初始化

string g(10,'x');//直接初始化
string h ("123456");//直接初始化

```

1. 默认初始化的默认值由变量的类型决定，同时定义变量的位置会有影响。

   内置类型：在函数体外初始化为0；在函数体内将不被初始化，变量的值是未定义的

   类的对象如果没有显式地初始化，则由其类默认构造函数确定。

2. 值初始化：

   vector\<string\> d(10)只提供了容纳的数量而省去初始值，库为每一个元素创建一个初值，这个初值的值由类型决定，如果是内置类型，则设为0，如果是某种类类型，则由类默认初始化，如果元素不支持默认初始化，则必须提供初始的元素值

   

##### 直接管理内存 new和delete

1. 由内置指针（不是智能指针）管理的动态内存在被显式释放前都会一直存在

2. delete之后重置指针值为nullptr，表明指针不指向任何对象

3. 使用new和delete管理动态内存的三个常见问题：

- 忘记delete
- 使用已经释放的对象
- 同一块内存释放两次

4. 坚持只使用智能指针就能避免这些问题

```c++
int * factory(int i)
{
return new int(i);//指针离开作用域，内存依然存在,调用者负责释放该内存
};
void use_factory()
{
        int * i=factory(10);
        delete i;//如果在delete之前发生异常，则内存不会释放
        i=nullptr;
}
int main()
{
        int * pi=new int;//默认初始化
        string * ps=new string;//默认初始化
        int *pi2=new int(1024);//直接初始化
        string *ps2=new string(10,'9');//直接初始化
        vector<int> *pv=new vector<int>{0,1,2,3,4,5,6,7,8,9};//列表初始化
        string * ps3=new string();//值初始化
        int *pi3=new int();////值初始化
        auto p1=new auto(10);//根据参数的类型来决定p1的类型
        const int *pci=new const int(1024);//直接初始化
        const string * pcs=new const string;//默认初始化
        use_factory();
        delete pi;delete pi2;delete pi3;
        delete ps;delete ps2;delete ps3;
        delete p1;delete pv;delete pci;delete pcs;
}
```



#### shared_ptr 和new 结合使用

1. 不能将一个内置指针隐式转换成智能指针，必须使用直接初始化

```c++
shared_ptr<int> clone (int p)
{
    //return new int(p);//错误
    return shared_ptr<int>(new int(p));
};
```

2. 默认情况下，一个用来初始化智能指针的普通指针必须指向动态内存，因为智能指针默认使用delete释放它关联的对象，当然也可以定义自己的操作来代替delete

3. 不要混合使用智能指针和普通指针         

```c++
void process(shared_ptr<int>ptr)
{
};
int main(){
shared_ptr<int> p(new int(10));
process(p);//正确，
int i=*p;
cout<<"afer process,*p="<<i<<endl;

int *x=new int (23);
process(shared_ptr<int>(x));//错误：智能指针离开作用域，引用计数为0，析构了x
int j=*x;//未定义的：x是一个悬空指针
cout<<"afer process,*p="<<j<<endl;
}
```

4. get用来将指针的访问权传递给代码，要确定代码不会delete指针的情况下，才能使用get，特别是永远不要使用get初始化另外一个智能指针或为指针指针赋值

````c++
shared_ptr<int> p(new int(10));
int *q=p.get();
{//新程序块
    shared_ptr<int>(q);
}//程序块结束，q被销毁，它指向的内存被是释放
int foo=*p;//未定义：p指向的内存已经被释放
cout<<"*p="<<j<<endl;
````

##### 其他shared_ptr操作

| 定义和改变shared_ptr的其他方法                               |
| ------------------------------------------------------------ |
| shared_ptr\<T\> p(q)  p管理q内置指针所指向的对象             |
| shared_ptr\<T\> p(u) 从unqiue_ptr u接管对象的所有权，将u置为空 |
| shared_ptr\<T\> p(p2,d)p是p2的拷贝，调用d释放                |
| shared_ptr\<T\> p(q,d )管理q内置指针所指向的对象，调用d释放q，不会delete |
| p.reset()   p指向空对象，如果p是唯一指向对象的shared_ptr，会释放p指向的对象   p.reset(q)  q是内置指针，如果定义了q，p会指向q  p.reset(q,d) 若还传递了d，会调用d释放q，不会delete |

```c++
void f()
{
        int *ip=new int(32);
        delete ip;//记得释放delete内存

}
shared_ptr<int> clone (int p)
{
        //return new int(p);错误：不能将一个内置指针隐式转换成智能指针
        return shared_ptr<int>(new int(p));
};
int main()
{
        shared_ptr<int> p1(new int(1024));
//      shared_ptr<int> p2=new int(1024);//错误：不能将一个内置指针隐式转换成智能指针
        auto p2=p1;
        f();
        if(!p1.unique())
                p1.reset(new int (30));
        *p1+=10;
        cout<<*p1<<endl;
        //unique_ptr<string> p3(new string("this is unique_ptr"));
        //shared_ptr<string> p4(p3);//注释的这两行是错误：p4调用了拷贝构造，而p3是unique_ptr，不能拷贝
        shared_ptr<string> p4(unique_ptr<string>(new string("unique_ptr string")));
        cout<<*p4<<endl;
}

```



##### 智能指针与异常

如果使用new 和delte管理内存，在delete之前发生异常，则内存不会释放。而使用智能指针能够避免这种情况，



##### unique_ptr

只能有一个unique_ptr指向一个对象，unique_ptr不支持普通的拷贝和赋值操作，可以通过release或reset将指针的所有权从一个（非const）unique_ptr转移到另外一个unique_ptr

| unique_ptr特有操作                                           |
| ------------------------------------------------------------ |
| unique_ptr\<T\> u1 ;unique_ptr\<T,D\> u2; 空unique_ptr，u1会使用delete释放指针, u2使用一个类型为D的可调用对象 |
| unique_ptr\<T,D\> u(d) 空unique_ptr使用类型为D的对象d代替delete |
| u=nullptr;释放u指向的对象，将u置为空                         |
| u.release();u放弃对指针的控制权，返回指针，并将u置为空       |
| u.reset();释放u指向的对象；u.reset(q);如果提供了内置指针，还会令u指向这个对象，否则u置为空 reset(nullptr); |

1. 我们可以拷贝或赋值一个将要销毁的unique_ptr

```c++
unique_ptr<string> clone(string p)
{
    return unique_ptr<string>(new string(p));
}
int main()
{
    unique_ptr<string> p1(new string("p1 unique_ptr"));
    //unique_ptr<string> p2(p1);// 错误：unique_ptr(const unique_ptr&) = delete
    unique_ptr<string> p3;
    //p3=p1; //错误：unique_ptr& operator=(const unique_ptr&) = delete
    unique_ptr<string> p2(p1.release());//将p1指向的对象的控制权转移给p2,并将p1置为空
    if(p1) cout<<*p1<<endl;
    unique_ptr<string> p4(new string("p4 unique_ptr"));
    p2.reset(p4.release());//p2释放原来指向的对象，管理p4指向的对象
    cout<<*p2<<endl;
    auto p5=clone("copy unique_ptr");//可以拷贝或赋值一个将要销毁的unique_ptr
    cout<<*p5<<endl;
}

```



##### 向unique_ptr传递删除器

1. unique_ptr的删除器定义与shared_ptr不一样

```c++
struct connection
{
    connection(string i,int p):ip(i),port(p){};
    string ip;
    int port;
};
connection connect(string ip,int port)
{
    connection* con=new connection(ip,port);
    return *con;
};
void close_connection(connection* con)
{
    cout<<"close connection:"<<con->ip<<":"<<con->port<<endl;
};
int main(){
    connection c=connect("127.0.0.1",80);
    unique_ptr<connection,decltype(close_connection)*> conp(&c,close_connection);
    //shared_ptr<connection> conp1(&c,close_connection);//shared_ptr删除器的定义
    //离开作用域，unique_ptr被销毁，连接会调用自定义的方式关闭连接
}

```



##### weak_ptr

是一种不控制所指向对象生存期的智能指针，它指向一个由shared_ptr管理的对象。一个weak_ptr绑定一个shared_ptr，不会改变shared_ptr的引用计数。

| weak_ptr                                                     |
| ------------------------------------------------------------ |
| weak_ptr\<T\> w 空weak_ptr可以指向类型为T的对象              |
| weak_ptr\<T\> w(sp) 与shared_ptr sp指向相同对象的weak_ptr    |
| w=p p可以weak_ptr或者weak_ptr                                |
| w.use_count() 与w共享对象的shared_ptr的数量                  |
| w.reset() 将w值为空                                          |
| w.expired() 返回w.use_count()是否等于0                       |
| w.lock() 如果expired为true，返回一个空的shared_ptr，否则返回一个指向一个w的对象的shared_ptr |

1. 由于对象可能不存在，我们不能使用weak_ptr直接访问对象，而必须使用lock检查weak_ptr指向的对象是否存在

```c++
int main()
{
    auto p=make_shared<string>("pp");
    weak_ptr<string> wp(p);
    weak_ptr<string> wp2=p;
    weak_ptr<string> wp3=wp;
    cout<<"p.use_count:"<<p.use_count()<<endl;
    //weak_ptr没有影响shared_ptr的引用计数
    wp2.reset();
    wp3.reset();
    if(!wp2.lock()&&!wp3.lock())
        cout<<"wp2 and wp3 is nullptr after calling reset"<<endl;
    if(shared_ptr<string> np=wp.lock())
    {
        cout<<"np:"<<*np<<endl;
        cout<<"np.use_count:"<<np.use_count()<<endl;
    }

}
```



#### 动态数组

##### new和动态数组

1. 虽然我们可以 要空括号对数组中的元素进行值初始化，但不能在括号中给出初试化值

```c++
int *a=new int[10]();//正确
int *a=new int[10](10);//错误
```

2. 不能创建一个大小为0的静态数组，但可以new一个大小为0的数组，返回一个合法的非空指针，可以像后尾迭代器一样使用，但不能解引用-毕竟它不指向任何元素

 ```c++
int a[0];//错误
int *a=new int[0];//正确
 ```

4. 释放动态数组，delete一定要加[]


```c++
int main()
{
    int * pia=new int[10];
    int *pia2=new int[10]();
    //int *pia2=new int[10](10);//错误
    string  *pia3=new string[10]{"a","a","the",string(3,'x')};
    for(string *p=pia3;p!=pia3+10;p++)
        cout<<*p<<endl;
    delete []pia;
    delete []pia2;
    delete []pia3;
}
```




##### 智能指针和动态数组

1. 指向数组的unique_ptr不支持成员访问运算符（点和箭头），支持下标[]

2. shared_ptr必须提供自己的删除器来管理动态数组。如果未提供，将使用delete而不是delete[]

3. shared_ptr未定义下标运算符，而且智能指针不支持指针算术运算，因此只能使用get来获取内置指针

```c++
int main()
{
    unique_ptr<int[]> up(new int[10]);
    for(int i=0;i<10;i++)
        up[i]=i;
    up.release();//自动调用delete[] 销毁指针
    shared_ptr<int> sp(new int [10],[](int *p){delete [] p;cout<<"delete []"<<endl;});
    for(int i=0;i<10;i++)
        *(sp.get()+i)=i;
    sp.reset();

}
```



##### allocator 类

new有一些灵活的限制，将内存分配和对象的构造组合在一起，delete将内存释放和对象的析构组合一起

```c++
int main()
{
int n=5;
string * const p=new string[n];
string *q=p;
string s;
while(cin>>s&&q!=p+n)
        *q++=s;
delete [] p;
   // 元素被赋值了两次：一次是默认初始化，一次是赋值时
}
```

如果希望将内存分配和对象构造分离，在真正需要才执行对象创建，allocator类可以做到。

| allocator类                                                  |
| ------------------------------------------------------------ |
| allocate\<T\> a 定义一个allocator对象，可以为T类型对象分配内存 |
| a.allocate(n) 分配一段未构造的原始的内存                     |
| a.dealallocate(p,n)释放内存p指针开始，n个对象的内存，p必须是allocate()返回的指针，n是创建p时的大小 |
| a.construct(p,args) p必须是T*类型的指针，指向一块原始内存，args传递给类型为T的构造函数 |
| a.destroy(p) 对p指向的对象执行析构函数                       |

1. 为了使用allocate返回的内存，必须使用construct构造对象，访问未构造的内存，行为是未定义的

2. 只能对真正构造了的元素进行destroy操作

| allocator算法                                                |
| ------------------------------------------------------------ |
| uninitialized_copy(b,e,b2) 从迭代器b和e指定的范围拷贝元素到迭代器b2指定的原始内存 |
| uninitialized_copy_n(b,n,b2) 从迭代器b开始，拷贝n个元素到迭代器b2指定的原始内存 |
| uninitialized_fill(b,e,t)     在迭代器b和e指向的原始内存范围内，创建对象，值为t的拷贝 |
| uninitialized_fill_n(b,n,t) 从迭代器b指向的原始内存开始，创建n个对象，值为t的拷贝 |

```c++
int main()
{
    int n=10;
    allocator<string> alloc;
    string * const p=alloc.allocate(n);
    auto q=p;
    alloc.construct(q++);
    alloc.construct(q++,10,'c');
    alloc.construct(q++,"hi");
    cout<<*(p+1)<<endl;
    //cout<<*q<<endl;//错误，问未构造的内存，行为是未定义的
    vector<string> vi{"1","2","3","4","5"};
    auto q1=uninitialized_copy(vi.begin(),vi.end(),q);
    uninitialized_fill_n(q1,2,"fill");
    for(int i=0;i<n;i++)
        cout<<*(p+i)<<endl;
    while(q!=p)
         alloc.destroy(--q);     
   // alloc.deallocate(p,n);// 也可以调用deallocate释放所有内存
}
```

#### 总结：现代c++程序应尽可能使用智能指针

