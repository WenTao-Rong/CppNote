#include<string>
#include<vector>
#include<iostream>
#include<memory>
using namespace std;
unique_ptr<string> clone(string p)
{
	return unique_ptr<string>(new string(p));
}
int main()
{
	auto p=make_shared<string>("pp");
	weak_ptr<string> wp(p);
	weak_ptr<string> wp2=p;
	weak_ptr<string> wp3=wp;
	cout<<"p.use_count:"<<p.use_count()<<endl;
	wp2.reset();
	wp3.reset();
	if(!wp2.lock()&&!wp3.lock())
		cout<<"wp2 and wp3 is nullptr after calling reset"<<endl;
	if(shared_ptr<string> np=wp.lock())
	{
		cout<<"np:"<<*np<<endl;	
		cout<<"np.use_count:"<<np.use_count()<<endl;
	}

}
