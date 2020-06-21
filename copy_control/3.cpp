#include<iostream>
#include<string>
using namespace std;
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
