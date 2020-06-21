#include<memory>
#include<iostream>
#include <string>
struct	X
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
	std::cout<<"x: "<<x.s<<" x2:"<<x2.s<<std::endl;
	hasX hx,hx2=std::move(hx);
}
