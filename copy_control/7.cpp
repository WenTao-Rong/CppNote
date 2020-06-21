#include<memory>
#include<iostream>
#include<utility>
#include<string>
#include<cstddef>
class StrVec
{
	public:
		StrVec():elements(nullptr),cap(nullptr),first_free(nullptr){};
		~StrVec(){
		free();
		}
		StrVec(const StrVec& s)
		{
			auto data=alloc_n_copy(s.begin(),s.end());
			elements=data.first;
			first_free=cap=data.second;
			
		}
		
		StrVec& operator =(const StrVec &s)
		{
			auto data=alloc_n_copy(s.begin(),s.end());
			free();
			elements=data.first;
			first_free=cap=data.second;
			return *this;

		}
		StrVec(StrVec&& s) noexcept:elements(s.elements),cap(s.cap),first_free(s.first_free)
		{
			std::cout<<"move construct"<<std::endl;	
			s.elements=s.first_free=s.cap=nullptr;
		}
		StrVec& operator= (StrVec&& rhs) noexcept
		{
			if(this!=&rhs)
			{
				free();
				elements=rhs.elements;
				first_free=rhs.first_free;
				cap=rhs.cap;
				rhs.elements=rhs.first_free=rhs.cap=nullptr;
			}
			return *this;
		}	
		void push_back(const std::string& s)
		{
			check_alloc();
			std::cout<<"push_back copy version"<<std::endl;	
			alloc.construct(first_free++,s);
		}
		void push_back(std::string &&s)
		{
			std::cout<<"push_back move version"<<std::endl;	
			check_alloc();
			alloc.construct(first_free++,std::move(s));
		}
		size_t size()const {return first_free-elements;}
		size_t capacity() const {return cap-elements;};
		std::string* begin() const {return elements;}
		std::string* end() const {return first_free;}
		
	private:
		std::pair<std::string* ,std::string*>  alloc_n_copy(const std::string * begin,const  std::string * end)
		{
			auto data=alloc.allocate(end-begin);
			return {data,uninitialized_copy(begin,end,data)};
		}
		void reallocate()
		{
			int newcapcity=size()?2*size():1;
			std::string * newdata=alloc.allocate(newcapcity);
			std::string* dest=newdata;
			std::string* elem=elements;
			/*for(int i=0;i<size();i++)
			{
				alloc.construct(dest++,std::move(*elem++));
			}*/
			 uninitialized_copy(make_move_iterator(begin()),make_move_iterator(end()),dest);
			free();
			elements=newdata;
			first_free=dest;
			cap=newdata+newcapcity;
		}
		void check_alloc(){if(size()==capacity()) reallocate();}
		void free()
		{
			if(elements)
			{
				for(auto p=first_free;p!=elements;p--)
					alloc.destroy(p);
				alloc.deallocate(elements,cap-elements);
			}
		}
		std::string *elements;
		std::string *cap;
		std::string *first_free;
		static std::allocator<std::string> alloc;

		
};
std::allocator<std::string> StrVec::alloc;
int main()
{
	StrVec p;
	p.push_back("string1");
	std::cout<<p.size()<<std::endl;
	p.push_back("string2");
	p.push_back("string2");
	std::string s="l val string";
	p.push_back(s);
	std::cout<<p.capacity()<<std::endl;
	StrVec p2(p);
	StrVec p3=p2;
        StrVec p4=std::move(p3);
	std::cout<<p2.size()<<std::endl;
        std::cout<<p3.size()<<std::endl;
        std::cout<<p4.size()<<std::endl;
}
