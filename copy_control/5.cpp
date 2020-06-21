#include<iostream>
#include<string>
using namespace std;
class HasPtr
{
	public:
		HasPtr(const std::string  & s=std::string()):ps(new std::string(s)),i(0),use(new size_t(1)){};	
		~HasPtr(){

			if(--*use==0) {delete ps;delete use;cout<<"HasPtr deconstruct"<<endl;}

		}
		HasPtr(const HasPtr& p):ps(p.ps),i(p.i){++*use;}
		HasPtr& operator =(const HasPtr& rhs)
		{
			++*rhs.use;
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
int main()
{
	HasPtr p("some value");
	p.print();
	HasPtr p2(p); 

}
