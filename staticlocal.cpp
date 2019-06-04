/*-pthread -lrt*/
#include <iostream>
#include <string>
#include <thread>
#include <mutex>

#include <mqueue.h>
#include <sys/stat.h>
#include <fcntl.h>

using std::cout;
using std::endl;
using std::mutex;
using std::thread;
using std::lock_guard;
using std::mutex;
using std::string;

mutex gd_mutex;

void func()
{
	static int flag = 0;
	
	int flag_l = 0;
	while(flag<100)
	{
		lock_guard<mutex> lg(gd_mutex);
		cout<<std::this_thread::get_id()<<": "<<flag++<<" "<<flag_l<<endl;
		//gd_mutex.unlock();
	}

}



int main(int argc,char *argv[])
{
	mode_t perms;
	mqd_t mqd;
	struct mq_attr attr,*attrp;

	attrp = NULL;
	attr.mq_maxmsg =20;
	attr.mq_msgsize = 2048;

	int flags = O_RDWR|O_CREAT;
	perms = S_IRUSR|S_IWUSR;
	
	string pathname = "tmp";

	mqd = mq_open(pathname.c_str(),flags,perms,attrp);

	if(mqd == (mqd_t)-1 )
	{
		cout<<"mqd create error!!!"<<endl;
		return 0;
	}	

	thread t1(func);
	thread t2(func);

	t1.join();
	t2.join();

	return 0;
}
