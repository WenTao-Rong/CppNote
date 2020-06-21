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
