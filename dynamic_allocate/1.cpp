#include<string>
#include<vector>
#include<iostream>
#include<memory>
#include<list>
using namespace std;
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
	if(p1) cout<<*p1<<endl;
	shared_ptr<int> p3=make_shared<int>(42);
	shared_ptr<string> p4=make_shared<string>(10,'9');
	shared_ptr<int> p5=make_shared<int>();
	auto p6=make_shared<vector<string>>();
	auto q(p3);
	cout<<"q.use_count"<<q.use_count()<<endl;
	auto r=make_shared<int>(50);
	r=q;	
	shared_ptr<Foo> foo=factory();
	cout<<"q.use_count"<<q.use_count()<<endl;
}
