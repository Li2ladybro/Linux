#include "Thread.hpp"

#include <memory>
#include <unistd.h>

// void *thread_run(void *argv)
// {
//     std::string workType = (const char *)argv;
//     while (1)
//     {
//         std::cout << "我正在做一个新线程 " << workType << std::endl;
//         sleep(1);
//     }
// }

// 共享资源，火车票
int tickets = 10000;

// 就需要尽可能的让多个线程交叉执行
// 多个线程交叉执行本质:就是让调度器尽可能的频繁发生线程调度与切换
// 线程一般在什么时候发生切换呢?时间片到了，来了更高优先级的线程，线程等待的时候。
// 线程是在什么时候检测上面的问题呢?从内核态返回用户态的时候，线程要对调度状态进行检测，如果可以，就直接发生线程切换

void *getTicket(void *usrname)
{
    while (1)
    {
        if (tickets > 0)
        {
            usleep(1234);

            std::cout << (char *)usrname << " 正在进行抢票：" << tickets-- << std::endl;
        }
        else
        {
            break;
        }
    }
    return nullptr;
}

int main()
{
    // std::unique_ptr<Thread> thread1(new Thread(thread_run,
    //                                            (void *)"hello thread", 1));
    // std::unique_ptr<Thread> thread2(new Thread(thread_run,
    //                                            (void *)"count thread", 2));
    // std::unique_ptr<Thread> thread3(new Thread(thread_run,
    //                                            (void *)"log thread", 3));

    // thread1->join();
    // thread2->join();
    // thread3->join();
    std::unique_ptr<Thread> thread1(new Thread(getTicket,
                                               (void *)"user1", 1));
    std::unique_ptr<Thread> thread2(new Thread(getTicket,
                                               (void *)"user2", 2));
    std::unique_ptr<Thread> thread3(new Thread(getTicket,
                                               (void *)"user3", 3));
    std::unique_ptr<Thread> thread4(new Thread(getTicket,
                                               (void *)"user4", 4));
    std::unique_ptr<Thread> thread5(new Thread(getTicket,
                                               (void *)"user5", 5));
    thread1->join();
    thread2->join();
    thread3->join();
    thread4->join();
    thread5->join();

    return 0;
}