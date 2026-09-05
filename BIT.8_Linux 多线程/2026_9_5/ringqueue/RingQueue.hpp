// 环形队列-消费者与生产者模型
#pragma once

#include <cassert>

#include <iostream>

#include <vector>

#include <semaphore.h>
#include <pthread.h>

static const int gcap = 1000; // 默认空间
template <class T>
class RingQueue
{

private:
    void P(sem_t &sem)
    {
        sem_wait(&sem);
    }

    void V(sem_t &sem)
    {
        sem_post(&sem);
    }

public:
    RingQueue(const int &cap = gcap)
        : _queue(cap), _cap(cap)
    {
        assert(sem_init(&_emptySemaphore, 0, _cap) == 0);
        assert(sem_init(&_dataSemaphore, 0, 0) == 0);

        pthread_mutex_init(&_pmutex, nullptr);
        pthread_mutex_init(&_cmutex, nullptr);

        _producerStep = _consumerStep = 0;
    }

    void Push(const T &in) // 输入型参数
    {
        pthread_mutex_lock(&_pmutex);

        P(_emptySemaphore); // 申请到了空位置

        // 将数据放入共享资源
        _queue[_producerStep++] = in;
        _producerStep %= _cap;

        V(_dataSemaphore); // 释放一个数据资源

        pthread_mutex_unlock(&_pmutex);
    }

    void Pop(T *out) // 输出型参数
    {
        pthread_mutex_lock(&_cmutex);

        P(_dataSemaphore); // 申请到了资源

        // 从共享资源中弹出一个数据资源
        *out = _queue[_consumerStep++];
        _consumerStep %= _cap;

        V(_emptySemaphore); // 释放一个空位置资源

        pthread_mutex_unlock(&_cmutex);
    }

    ~RingQueue()
    {
        sem_destroy(&_emptySemaphore);
        sem_destroy(&_dataSemaphore);

        pthread_mutex_destroy(&_cmutex);
        pthread_mutex_destroy(&_pmutex);
    }

private:
    std::vector<T> _queue; // 队列
    int _cap;              // 队列容量

    sem_t _emptySemaphore; // 生产者，看中是否有空位置
    sem_t _dataSemaphore;  // 消费者，看中有无资源

    pthread_mutex_t _pmutex; // 生产者的锁
    pthread_mutex_t _cmutex; // 消费者的锁

    int _producerStep;
    int _consumerStep;
};
