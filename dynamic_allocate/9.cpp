#include<string>
#include<vector>
#include<memory>
#include<iostream>
using namespace std;
int main()
{
	int n=10;
	allocator<string> alloc;
	string * const p=alloc.allocate(n); 
	auto q=p;
	alloc.construct(q++);
	alloc.construct(q++,10,'c');
	alloc.construct(q++,"hi");
	cout<<*(p+1)<<endl;
	//cout<<*q<<endl;
	vector<string> vi{"1","2","3","4","5"};
	auto q1=uninitialized_copy(vi.begin(),vi.end(),q);
	uninitialized_fill_n(q1,2,"fill");
     	for(int i=0;i<n;i++)
		cout<<*(p+i)<<endl;	
     	//while(q!=p)
	//	alloc.destroy(--q);	
	alloc.deallocate(p,n+1);// return all cache
	
}
