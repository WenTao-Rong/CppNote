#include<string>
#include<vector>
#include<iostream>
#include<memory>
using namespace std;
struct connection
{
	connection(string i,int p):ip(i),port(p){};
	string ip;
	int port;
};
connection connect(string ip,int port)
{
	 connection* con=new connection(ip,port);
	 return *con;
};
void close_connection(connection* con)
{
	cout<<"close connection:"<<con->ip<<":"<<con->port<<endl;
};
unique_ptr<string> clone(string p)
{
	return unique_ptr<string>(new string(p));
};
int main()
{
	unique_ptr<string> p1(new string("p1 unique_ptr"));
	//unique_ptr<string> p2(p1); unique_ptr(const unique_ptr&) = delete
	unique_ptr<string> p3;
	//p3=p1; unique_ptr& operator=(const unique_ptr&) = delete
	unique_ptr<string> p2(p1.release());
	if(p1) cout<<*p1<<endl;
	unique_ptr<string> p4(new string("p4 unique_ptr"));
	p2.reset(p4.release());
	cout<<*p2<<endl;
	auto p5=clone("copy unique_ptr");
	cout<<*p5<<endl;
	
	connection c=connect("127.0.0.1",80);
	unique_ptr<connection,decltype(close_connection)*> conp(&c,close_connection);
	//shared_ptr<connection> conp1(&c,close_connection);
};
