//线程之间传递参数和异常
//promise : store value or exception only one 
#include <iostream>
#include <thread>
#include <future>
#include <exception>
#include <stdexcept>
#include <functional>
#include <utility>

using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::cerr;

void doSomething(std::promise<string>& p)
{
	try{
		//read character and throw exception if 'x'
		cout<<"read char ('x' for exception)"<<endl;

		char c = cin.get();

		if(c == 'x')
		{
			throw std::runtime_error(string("char")+c+" read");
		}
		
		string s = string("char")+c+" processed";
		p.set_value(std::move(s));//store result

	}catch(...){
		p.set_exception(std::current_exception());//store exception
	}
}

int main()
{
	try{
		//start thread using a promise to store the outcome 
		std::promise<string> p;
		std::thread t(doSomething,std::ref(p));
		t.detach();
		//creat a future to process the outcome
		std::future<string> f(p.get_future());

		//process the outcome
		//untill shared_state is true
		cout<<"result: "<<f.get()<<endl;

	}catch(const std::exception& e){
		cerr<<"EXCEPTION"<<e.what()<<endl;
	}catch(...){
		cerr<<"EXCEPTION"<<endl;
	}
	
	return 0;
}
