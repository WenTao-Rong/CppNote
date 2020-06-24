#include<string>
#include<vector>
#include<iostream>
#include<memory>
using namespace std;
void f()
{
	int *ip=new int(32);
	delete ip;
	
}
void process(shared_ptr<int>ptr)
{
};

shared_ptr<int> clone (int p)
{
	//return new int(p);
	return shared_ptr<int>(new int(p));
};
int main()
{
	shared_ptr<int> p(new int(10));
	process(p);
	int i=*p;
	cout<<"afer process,*p="<<i<<endl;
	int *x=new int (23);
	process(shared_ptr<int>(x));
	int j=*x;
	cout<<"afer process,*p="<<j<<endl;
	
	int *q=p.get();
	{
		shared_ptr<int>(q);
	}
	int foo=*p;
	cout<<"*p="<<j<<endl;
	shared_ptr<int> p1(new int(1024));

//	shared_ptr<int> p2=new int(1024);
        
	auto p2=p1;
	f();
	if(!p1.unique())
    		p1.reset(new int (30));
	*p1+=10;
	cout<<*p1<<endl;
//	unique_ptr<string> p3(new string("this is unique_ptr"));
//	shared_ptr<string> p4(p3);
	shared_ptr<string> p4(unique_ptr<string>(new string("unique_ptr string")));
	cout<<*p4<<endl;
}
