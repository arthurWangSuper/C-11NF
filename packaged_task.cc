// 类模板 std::packaged_task 包装任何可调用callable目标(函数，lambda表达式，bind表达式或其他函数对象)，使得能异步调用它。
// 其返回值或所抛异常被存储于能通过std::future 对象访问的共享状态中
#include <iostream>
#include <cmath>
#include <thread>
#include <future>
#include <functional>

// 避免对 std::pow 重载集消歧义的独有函数
int f(int x,int y) {return std::pow(x,y);}

void task_lambda(){
    std::packaged_task<int(int,int)> task([](int a,int b){
        return std::pow(a,b);
    });
    std::future<int> result = task.get_future();

    task(2,9);
    std::cout<<"task_lambda:\t"<<result.get()<<'\n';
}

void task_bind(){
    std::packaged_task<int()> task(std::bind(f,2,11));
    std::future<int> result = task.get_future();

    task();

    std::cout<<"task_bind:\t"<<result.get()<<std::endl;
}

void task_thread()
{
    std::packaged_task<int(int,int)> task(f);
    std::future<int> result = task.get_future();

    std::thread task_td(std::move(task),2,10);
    task_td.join();

    std::cout<<"task_thread:\t"<<result.get()<<'\n';
}

int main()
{
    task_lambda();
    task_bind();
    task_thread();
}
