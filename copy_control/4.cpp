#include<string>
#include<memory>
#include<vector>
#include<iostream>
using namespace std;
class Sale_data
{
	public:
		Sale_data(string book,int  units,double reven):bookNo(book),units_sold(units),revenue(reven)
	{};
		Sale_data(const Sale_data & org):bookNo(org.bookNo),units_sold(org.units_sold),revenue(org.revenue){}
		Sale_data & operator =(const Sale_data &rhs)
		{
			bookNo=rhs.bookNo;
			units_sold=rhs.units_sold;
			revenue=rhs.revenue;
			return *this;
		}
		~Sale_data()
		{
			cout<<"Sale_data deconstruct"<<endl;
		}
		void print()
		{
			cout<<"bookno: "<<bookNo<<" unit_sold: "<<units_sold<<" revenue: "<<revenue<<endl;
		};

	private:
		string bookNo;
		int units_sold=0;
		double revenue=0.0;


};
int main()
{
 Sale_data * p=new Sale_data("book1",23,4.3);
auto p2=make_shared<Sale_data>("book2",45,9.3);
Sale_data item(*p);
vector<Sale_data> vec;
vec.push_back(*p2);
delete p;
}
