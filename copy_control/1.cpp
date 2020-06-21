#include<string>
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
       
	Sale_data s1("bookk",5,9.34);
	Sale_data s2=s1;
	s2.print();
	Sale_data s3(s1);
	s3.print();
	
	
	
}
