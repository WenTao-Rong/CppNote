#include<iostream>
#include<string>
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
		}
		friend void swap(HasPtr & lhs,HasPtr &rhs);
		void print(){std::cout<<"*ps= "<<*ps<<" i="<<i<<std::endl;}

	private:
		std::string *ps;
		int i;
};
inline void swap(HasPtr & lhs,HasPtr &rhs)
{
	using std::swap;
	swap(lhs.ps,rhs.ps);
	swap(lhs.i,rhs.i);
};
int main()
{
	HasPtr p("jsdf");
	HasPtr q=p;
	q.print();

}
