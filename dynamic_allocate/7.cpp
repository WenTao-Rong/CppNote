#include<string>
#include<iostream>
#include<vector>
#include<memory>
using namespace std;

int main()
{
	unique_ptr<int[]> up(new int[10]);
	for(int i=0;i<10;i++)
		up[i]=i;
	up.release();
	shared_ptr<int> sp(new int [10],[](int *p){delete [] p;cout<<"delete []"<<endl;});
	for(int i=0;i<10;i++)
		*(sp.get()+i)=i;
	sp.reset();
	
}

