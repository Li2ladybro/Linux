#include "Mutex.hpp"

#include <string>
#include <vector>

#include <unistd.h>

int tickets = 1000;

// 定义一个全局锁
pthread_mutex_t gLock = PTHREAD_ADAPTIVE_MUTEX_INITIALIZER_NP;

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
        {
            LockGuard lockGuard(&gLock);
            if (tickets > 0)
            {
                usleep(12344);
                std::cout << ((ThreadData *)argv)->_threadName << " 正在抢票" << " : " << tickets-- << std::endl;
            }
            else
            {
                break;
            }
        }
        usleep(1000);
    }
    return nullptr;
}

int main()
{

#define NUM 4

    std::vector<pthread_t> tids(NUM);

    for (int i = 0; i < NUM; ++i)
    {
        char buffer[64];
        snprintf(buffer, sizeof buffer, "thread-%d", i + 1);
        ThreadData *td = new ThreadData(buffer, &gLock);
        pthread_create(&tids[i], nullptr, getTickets, td);
    }
    for (auto &e : tids)
    {
        pthread_join(e, nullptr);
    }


    return 0;
}