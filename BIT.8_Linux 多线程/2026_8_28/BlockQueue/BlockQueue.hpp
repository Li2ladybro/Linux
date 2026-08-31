#pragma

#include <iostream>

#include <queue>

#include <pthread.h>

template <class T>
class BlockQueue
{
public:
    // c++17
    inline static const int _gmaxCap = 500;

public:
    BlockQueue(const int &maxCap = _gmaxCap)
        : _maxCap(maxCap)
    {
        // 初始化锁以及条件变量
        pthread_mutex_init(&_mutex, nullptr);
        pthread_cond_init(&_producerCond, nullptr);
        pthread_cond_init(&_consumerCond, nullptr);
    }

    // 生产者调用
    void push(const T &in) // 输入型参数：const &
    {
        pthread_mutex_lock(&_mutex);

        // 1、做判断
        // if (isFull())
        // 细节2:充当条件判断的语法必须是while，不能用if
        // 标准范式：条件变量等待永远 while(条件) + wait，不要 if
        // 被唤醒后需要重新检测条件是否满足
        while (isFull())
        { 
            // 缓冲区满则等待，生产条件不足，无法生产，此时生产者进行等待

            // 细节1:pthread_cond_wait这个函数的第二个参数，必须是我们正在使用的互斥锁!
            // a.pthread_cond_wait:该函数调用的时候，会以原子性的方式，将锁释放，并将自己挂起
            // b.pthread_cond_wait:该函数在被唤醒返回的时候，会自动的重新获取你传入的锁

            // pthread_cond_wait 做两件原子操作：
            // - 释放 _mutex；
            // - 将当前线程挂起，等待条件变量。

            // 当线程被唤醒，函数返回前，会自动重新获取 _mutex，返回之后继续在临界区执行代码。

            // 互斥锁提前 lock，但是pthread_cond_wait调用时刻 主动释放锁，让别的线程可以进入临界区干活，所以不会死锁；
            // wait 返回时又自动重新拿到锁，继续执行临界区业务。
            // pthread_cond_signal仅仅发送唤醒通知，它本身不操作锁，不会造成死锁。

            pthread_cond_wait(&_producerCond, &_mutex);
        }

        // 2、走到这里没满，可以生产
        _q.push(in);

        // 3、绝对保障，阻塞队列里有数据，可以唤醒消费者线程

        // 细节3:pthread_cond_signal:这个函数，可以放在临界区内部，也可以放在外部
        pthread_cond_signal(&_consumerCond);

        pthread_mutex_unlock(&_mutex);
        // pthread_cond_signal(&_consumerCond);
    }

    // 消费者调用
    void pop(T *out) // 输出型参数：*
    {
        pthread_mutex_lock(&_mutex);

        // 1、做判断
        // if (isEmpty())
        // 如果只有一个生产者，许多消费者被大批唤醒，则需要用while
        while (isEmpty())
        {
            // 缓冲区空则等待，消费条件不足，无法消费，此时消费者进行等待
            pthread_cond_wait(&_consumerCond, &_mutex);
        }

        // 2、走到这里，一定不为空，可以消费
        *out = _q.front();
        _q.pop();

        // 3、绝对保障，阻塞队列里至少有一个空位，可以唤醒一个生产者线程
        pthread_cond_signal(&_producerCond);

        pthread_mutex_unlock(&_mutex);
    }

    ~BlockQueue()
    {
        // 销毁锁以及条件变量
        pthread_mutex_destroy(&_mutex);
        pthread_cond_destroy(&_producerCond);
        pthread_cond_destroy(&_consumerCond);
    }

private:
    bool isEmpty()
    {
        return _q.empty();
    }
    bool isFull()
    {
        return _q.size() == _maxCap;
    }

private:
    std::queue<T> _q;             // 阻塞队列
    int _maxCap;                  // 队列最大容量上限
    pthread_mutex_t _mutex;       // 用互斥锁保护阻塞队列
    pthread_cond_t _producerCond; // 生产者的条件变量  有空位置才可以生产
    pthread_cond_t _consumerCond; // 消费者的条件变量  有产品才可以消费
};
// template <class T>
// const int BlockQueue<T>::_gmaxCap;
