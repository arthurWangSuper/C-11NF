/* class thread api
* thread() noexcept;
* thread( thread&& other ) noexcept;
*
* template< class Function, class... Args >
* explicit thread( Function&& f, Args&&... args );
*
*/ thread(const thread&) = delete;

#include <iostream>
#include <utility>
#include <thread>
#include <chrono>

void f1(int n){
    for(int i=0;i<5;++i){
        //way to obtain pid in func no via thread::getid()
        std::cout<<"Thread 1 pid "<<std::this_thread::get_id()<<" executing"<<std::endl;
        ++n;
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

void f2(int& n) {
    for (int i = 0; i < 5; ++i) {
        //way to obtain pid in func no via thread::getid()
        std::cout << "Thread 2 pid" << std::this_thread::get_id() << " executing" << std::endl;
        ++n;
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

class foo{
public:
    void bar(){
        for(int i=0;i<5;++i){
            //way to obtain pid in func no via thread::getid()
            std::cout<<"Thread 3 pid "<<std::this_thread::get_id()<<" executing"<<std::endl;
            ++n;
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }
    int n = 0;
};

class baz{
public:
    void operator()(){
        for(int i=0;i<5;++i){
            //way to obtain pid in func no via thread::getid()
            std::cout<<"Thread 4 pid "<<std::this_thread::get_id()<<" executing"<<std::endl;
            ++n;
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }
    int n=0;
};

int main()
{
    int n=0;
    foo f;
    baz b;
    std::thread t1; //t1不是线程 
    std::thread t2(f1,n+1); //按值传递
    std::thread t3(f2,std::ref(n)); //按引用传递
    std::thread t4(std::move(t3));  //t4 现在运行f2,t3不再是线程
    std::thread t5(&foo::bar,&f); //t5在对象f上运行foo:bar
    std::thread t6(b); //t6在对象b上运行baz::operator()

    t2.join();
    t4.join();
    t5.join();
    t6.join();
    std::cout<<"Final value of n is "<<n<<std::endl;
    std::cout<<"Final value of foo::n is "<<f.n<<std::endl;

    return 0;
}
