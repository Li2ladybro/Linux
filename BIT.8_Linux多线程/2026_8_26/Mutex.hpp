#pragma once

#include <iostream>

#include <pthread.h>

class Mutex
{
public:
    Mutex(pthread_mutex_t *PLock = nullptr)
        : _pLock(PLock)
    {
    }
    ~Mutex()
    {
    }

    void lock()
    {
        if (_pLock != nullptr)
        {
            pthread_mutex_lock(_pLock);
        }
    }

    void unlock()
    {
        if (_pLock != nullptr)
        {
            pthread_mutex_unlock(_pLock);
        }
    }

private:
    pthread_mutex_t *_pLock; // 锁指针
};


// 锁护卫
class LockGuard
{

public:
    LockGuard(pthread_mutex_t *mutex = nullptr)
        : _mutex(mutex)
    {
        // 在构造中进行加锁
        _mutex.lock();
    }

    ~LockGuard()
    {
        // 在析构中进行解锁
        _mutex.unlock();
    }

private:
    Mutex _mutex;
};