#include<string>
#include<iostream>
#include<vector>
using namespace std;

int main()
{
	int * pia=new int[10];
	int *pia2=new int[10]();
	//int *pia2=new int[10](10);
	string  *pia3=new string[10]{"a","a","the",string(3,'x')};
	for(string *p=pia3;p!=pia3+10;p++)
		cout<<*p<<endl;	
	delete []pia;
	delete []pia2;
	delete []pia3;
}
