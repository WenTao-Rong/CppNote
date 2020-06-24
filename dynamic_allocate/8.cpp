#include<string>
#include<iostream>
using namespace std;
int main()
{
int n=0;
string * const p=new string[n];
string *q=p;
string s;
while(q!=p+n&&cin>>s)
	*q++=s;
delete [] p;
}
