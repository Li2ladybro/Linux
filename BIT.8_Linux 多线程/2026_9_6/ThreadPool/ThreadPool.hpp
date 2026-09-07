#pragma once

#include "Thread.hpp"
#include "Mutex.hpp"

#include <vector>
#include <queue>

#include <pthread.h>
#include <unistd.h>

#include <mutex>

using namespace ThreadNs;

const int gnum = 1000;

template <class T>
class ThreadPool;

template <class T>
struct ThreadData
{

    ThreadData(ThreadPool<T> *tp, const std::string &name)
        : _threadpool(tp), _name(name)
    {
    }
    ThreadPool<T> *_threadpool;
    std::string _name;
};

template <class T>
class ThreadPool
{
private:
    static void *headlerTask(void *args)
    {
        // ThreadPool<T> *tp = (ThreadPool<T> *)args;

        ThreadData<T> *td = (ThreadData<T> *)args;

        while (1)
        {
            T t;
            // sleep(1);
            // std::cout << "headlerTask thread " << pthread_self() << " run...\n";
            {
                LockGuard lockguard(td->_threadpool->mutex());
                // td->_threadpool->lockQueue();
                while (td->_threadpool->isQueueEmpty())
                {
                    td->_threadpool->threadWait();
                }
                // 获取任务
                t = td->_threadpool->pop(); // 弹出的本质，是将任务从共享资源中，弹入到当前的自己的独立栈中
                // td->_threadpool->unlockQueue();
            }
            // 处理任务
            std::cout << td->_name << " 获取了一个任务 " << t.toTaskString() << " 并处理完成结果为 " << t() << std::endl;
        }
        delete td;
        return nullptr;
    }

    ThreadPool(const int &num = gnum)
        : _num(num)
    {
        pthread_mutex_init(&_mutex, nullptr);
        pthread_cond_init(&_cond, nullptr);
        for (int i = 0; i < _num; ++i)
        {
            _threads.push_back(new Thread());
        }
    }

    void operator=(const ThreadPool &) = delete; // 赋值
    ThreadPool(const ThreadPool &) = delete;     // 拷贝构造

public:
    void lockQueue()
    {
        pthread_mutex_lock(&_mutex);
    }

    void unlockQueue()
    {
        pthread_mutex_unlock(&_mutex);
    }

    bool isQueueEmpty()
    {
        return _task_queue.empty();
    }

    void threadWait()
    {
        pthread_cond_wait(&_cond, &_mutex);
    }

    T pop()
    {
        T t = _task_queue.front();
        _task_queue.pop();
        return t;
    }

    pthread_mutex_t *mutex()
    {
        return &_mutex;
    }

public:
    void run()
    {
        for (auto &e : _threads)
        {
            ThreadData<T> *td = new ThreadData<T>(this, e->threadName());
            e->start(headlerTask, (void *)td);
            // std::cout << e->threadName() << " start ....\n";
        }
    }

    void push(const T &in)
    {
        LockGuard lockguard(&_mutex);
        // pthread_mutex_lock(&_mutex);
        _task_queue.push(in);
        pthread_cond_signal(&_cond);
        // pthread_mutex_unlock(&_mutex);
    }

    static ThreadPool<T> *getInstance()
    {
        if (_tp == nullptr) // 降低锁冲突概率，减少加锁解锁并发，提高性能
        {
            _tplock.lock(); // 防止并发错误
            if (_tp == nullptr)
            {
                _tp = new ThreadPool<T>();
            }
            _tplock.unlock();
        }
        return _tp;
    }

    ~ThreadPool()
    {
        pthread_cond_destroy(&_cond);
        pthread_mutex_destroy(&_mutex);
        for (auto &e : _threads)
        {
            delete e;
        }
    }

private:
    int _num; // 线程池线程总数

    std::vector<Thread *> _threads; // 线程池
    std::queue<T> _task_queue;      // 任务队列

    pthread_mutex_t _mutex;
    pthread_cond_t _cond;

    static ThreadPool<T> *_tp; // 类对象
    static std::mutex _tplock; // 保护类对象
};

template <class T>
ThreadPool<T> *ThreadPool<T>::_tp = nullptr;

template <class T>
std::mutex ThreadPool<T>::_tplock;