//编译时 需要添加-pthread 选项  multiple library
#include <iostream>
#include <thread>
#include <mutex>
#include <string>
#include <condition_variable>

using std::cout;
using std::endl;
using std::thread;
using std::string;
using std::ref;
using std::lock_guard;
using std::mutex;
using std::condition_variable;
using std::unique_lock;
using std::lock_guard;

condition_variable cv;
mutex m;
mutex k;

bool flag = false;


class stu
{
	public:
		stu(string stuname,unsigned age):name(stuname),age(age){};
		~stu(){};
		void getinfo() const;
	private:
		string name;
		unsigned age = 0;
};

void stu::getinfo() const
{
	cout<<"stu name is "<<this->name<<"age is "<<this->age<<endl;
}


void printstuinfo(stu &st)
{
	st.getinfo();
}
void hello(const string &str)
{
	cout<<"hello Cocurrent world"<<str<<endl;
}

void func1(string &str)
{
	cout<<"thread id: "<<std::this_thread::get_id()<<endl;
}
void addone(int &val)
{
	int cnt = 10;
	while(cnt>0)
	{
		//std::lock_guard<std::mutex> guard(m);
		lock_guard<mutex> lk(m);
		cout<<"thread id: "<<std::this_thread::get_id()<<"val = "<<val<<endl;
		val++;
		cnt--;
	}
}

void consumer(){
	unique_lock<mutex> lk(k);
	
	cv.wait(lk,[](){return flag?true:false;});

	cout<<"==consumer=="<<endl;
}
void producer(){
	lock_guard<mutex> lg(k);
	flag = true;
	cv.notify_all();
}
int main(int argc,char* argv[])
{
	//值传递 引用传递
	//到线程函数的参数被移动或按值复制。若需要传递引用参数给线程函数，则必须包装它（例如用 std::ref 或 std::cref ）。
	//忽略来自函数的任何返回值。若函数抛异常，则调用 std::terminate 。为将返回值或异常传递回调用方线程，可使用 std::promise 或 std::async 。 
	string str("thread");
	int val = 0;
	int &rval = val;	
 	stu *pst = new stu("ars",12);
	stu stuone(*pst);

	//thread t(hello,str);
	//thread t1(func1,ref(str));
	thread t2(addone,ref(rval));
	thread t3(addone,ref(rval));
	thread t4(printstuinfo,ref(stuone));
	thread t5(consumer);
	thread t6(producer);
	//t.join();
	//t1.detach();
	t2.join();
	t3.join();
	t4.join();
	t5.join();
	t6.join();

	cout<<"final val is "<<val<<endl;

	return 0;	

}
