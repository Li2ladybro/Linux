#include <iostream>

#include <string>

#include <pthread.h>
#include <unistd.h>

// 互斥资源
int tickets = 1000;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; // 互斥锁
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;    // 条件变量

void *start_routine(void *args)
{
    std::string name = (char *)args;
    while (1)
    {
        pthread_mutex_lock(&mutex);
        pthread_cond_wait(&cond, &mutex);
        // 判断暂时省掉
        std::cout << name << " -> " << tickets-- << std::endl;
        pthread_mutex_unlock(&mutex);
    }
    return nullptr;
}

int main()
{
    // 通过条件变量控制线程执行
    pthread_t t[5];
    for (int i = 0; i < 5; ++i)
    {
        char *name = new char[64];
        snprintf(name, 64, "thread %d", i + 1);
        pthread_create(t + i, nullptr, start_routine, name);
    }

    while (1)
    {
        sleep(1);
        // pthread_cond_signal(&cond);
        // std::cout << " main thread wakeup one thread\n";
        
        // 一键释放所有cond
        pthread_cond_broadcast(&cond);
    }

    for (int i = 0; i < 5; ++i)
    {
        pthread_join(t[i], nullptr);
    }

    return 0;
}