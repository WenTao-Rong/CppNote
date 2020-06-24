#include<string>
#include<vector>
using namespace std;
int * factory(int i)
{
return new int(i);
};
void use_factory()
{
	int * i=factory(10);
	delete i;
	i=nullptr;
}
int main()
{
	int * pi=new int;
	string * ps=new string;
	int *pi2=new int(1024);
	string *ps2=new string(10,'9');
	vector<int> *pv=new vector<int>{0,1,2,3,4,5,6,7,8,9};
	string * ps3=new string();
	int *pi3=new int();
	auto p1=new auto(10);
	const int *pci=new const int(1024);
	const string * pcs=new const string;
	use_factory();	
	delete pi;
	delete pi2;
	delete pi3;
	delete ps;
	delete ps2;
	delete ps3;
	delete p1;
	delete pv;
	delete pci;
	delete pcs;
}
