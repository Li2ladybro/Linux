#include <iostream>
#include <string>
#include <vector>

#include <pthread.h>
#include <unistd.h>

// 共享资源，火车票

// 定义一个全局锁
// pthread_mutex_initializer
// 静态初始化互斥锁宏

// 如何看待锁
// a. 锁，本身就是一个共享资源!全局的变量是要被保护的，锁是用来保护全局的资源的，锁本身也是全局资源，锁的安全谁来保护呢?
// b. 如何理解加锁和解锁的本质如果我们想简单的使用，该如何进行封装设计
// pthread_mutex_lock、pthread_mutex_unlock:加锁，解锁的的过程必须是安全的!加锁、解锁的过程其实是原子的!
// c. 如果申请成功，就继续向后执行，如果申请暂时没有成功，执行流会阻塞在当前位置!
// d. 谁持有锁就可以访问临界区

pthread_mutex_t gLock = PTHREAD_MUTEX_INITIALIZER;;

int tickets = 1000;

// 1.多个执行流进行安全访问的共享资源-临界资源
// 3.想让多个线程串行访问共享资源--互斥
// 2.我们把多个执行流中，访问临界资源的代码 --临界区--往往是线程代码的很小的一部分
// 4.对一个资源进行访问的时候，要么不做，要么做完--原子性，不是原子性的情况--一个对资源进行的操作，如果只用一条汇编就能完成 -- 原子性
// 反之:不是原子的 --当前理解，方便表述
// 提出解决方案:加锁!

class ThreadData
{
public:
    ThreadData(const std::string &threadName, pthread_mutex_t *pMutex)
        : _threadName(threadName), _pMutex(pMutex)
    {
    }
    ~ThreadData()
    {
    }

    std::string _threadName;
    pthread_mutex_t *_pMutex;
};

void *getTickets(void *argv)
{
    while (1)
    {
        // 抢票
        // 加锁解锁的过程是多个线程串行执行的
        // 锁只保证线程互斥访问，没有规定，谁先跑
        // 锁是多个线程竞争的结果

        // pthread_mutex_lock(((ThreadData *)argv)->_pMutex);
        pthread_mutex_lock(&gLock);
        pthread_mutex_lock(&gLock);

        if (tickets > 0)
        {
            usleep(12344);
            std::cout << ((ThreadData *)argv)->_threadName << " 正在抢票" << " : " << tickets-- << std::endl;
            // pthread_mutex_unlock(((ThreadData *)argv)->_pMutex);
            pthread_mutex_unlock(&gLock);
        }
        else
        {
            // pthread_mutex_unlock(((ThreadData *)argv)->_pMutex);
            pthread_mutex_unlock(&gLock);

            break;
        }
        // 在这里解锁可以吗？
        // pthread_mutex_unlock(((ThreadData *)argv)->_pMutex);

        // sleep(1); // 形成订单
    }
    return nullptr;
}

int main()
{

    // pthread_t t1, t2, t3, t4;

    // pthread_create(&t1, nullptr, getTickets, (void *)"user1");
    // pthread_create(&t2, nullptr, getTickets, (void *)"user2");
    // pthread_create(&t3, nullptr, getTickets, (void *)"user3");
    // pthread_create(&t4, nullptr, getTickets, (void *)"user4");

    // pthread_join(t1, nullptr);
    // pthread_join(t2, nullptr);
    // pthread_join(t3, nullptr);
    // pthread_join(t4, nullptr);

#define NUM 4

    // 定义一个局部锁
    pthread_mutex_t lock;
    pthread_mutex_init(&lock, nullptr);
    std::vector<pthread_t> tids(NUM);

    for (int i = 0; i < NUM; ++i)
    {
        char buffer[64];
        snprintf(buffer, sizeof buffer, "thread-%d", i + 1);
        ThreadData *td = new ThreadData(buffer, &lock);
        pthread_create(&tids[i], nullptr, getTickets, td);
    }
    for (auto &e : tids)
    {
        pthread_join(e, nullptr);
    }

    pthread_mutex_destroy(&lock);

    return 0;
}