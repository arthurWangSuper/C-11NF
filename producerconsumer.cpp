//
// Created by k on 19-8-1.
//

//问题在于：consumer 和 producer都处于等待的状态，只是判断条件符合而已，并没有发送信号？？？


// 1 线程在检查条件变量的状态时 锁定mutex
// 2 检查条件变量的状态 == 也就是说与预期符合的话，往下执行
// 3 如果条件变量的状态与预期不符，将线程进入睡眠并等待其他线程通知,并在进入睡眠前解锁
// 4 得到其他线程通知，再度上锁，继续执行。


#include <iostream>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>

int counter=0;
int maxSize = 30;
std::mutex mtx;
std::queue<int> dataQuene; // 被生产者和消费者共享

std::condition_variable producer, consumer;  // 条件变量是一种同步机制，要和mutex以及lock一起使用

void func_consumer()
{
    while (true)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));// 消费者比生产者慢

        std::unique_lock<std::mutex> lck(mtx);
        consumer.wait(lck, [] {return dataQuene.size() != 0; });     // 消费者阻塞等待， 直到队列中元素个数大于0
        int num=dataQuene.front();
        dataQuene.pop();
        std::cout << "consumer " << std::this_thread::get_id() << ": "<< num <<std::endl;

        producer.notify_all();                                       // 通知生产者当队列中元素个数小于maxSize
    }
}

void func_producer()
{
    while (true)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(900));  // 生产者稍微比消费者快
        std::unique_lock<std::mutex> lck(mtx);
        producer.wait(lck, [] {return dataQuene.size() != maxSize; });// 生产者阻塞等待， 直到队列中元素个数小于maxSize

        ++counter;
        dataQuene.push(counter);
        std::cout << "producer " << std::this_thread::get_id() << ": "<< counter <<std::endl;

        consumer.notify_all();                                        //通知消费者当队列中的元素个数大于0
    }
}

int main()
{
    std::thread consumers[2], producers[2];

    // 两个生产者和消费者
    for (int i = 0; i < 2; ++i)
    {
        consumers[i] = std::thread(func_consumer);
        producers[i] = std::thread(func_producer);
    }

    for (int i = 0; i < 2; ++i)
    {
        producers[i].join();
        consumers[i].join();
    }

    system("pause");
    return 0;
}
