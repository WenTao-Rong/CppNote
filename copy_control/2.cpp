#include<iostream>
#include<string>
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
	HasPtr ret=hp;
	return ret;
}
int main()
{
	HasPtr p("some value");
	f(p);
	HasPtr q(p);
	q.print();
	
}
