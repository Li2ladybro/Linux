#pragma

#include <iostream>

#include <queue>

#include <pthread.h>

template <class T>
class BlockQueue
{
public:
    // c++17
    inline static const int _gmaxCap = 5;

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
        if (isFull())
        {
            // 
            
            // 缓冲区满则等待，生产条件不足，无法生产，此时生产者进行等待
            pthread_cond_wait(&_producerCond, &_mutex);
        }

        // 2、走到这里没满，可以生产
        _q.push(in);

        // 3、绝对保障，阻塞队列里有数据，可以唤醒消费者线程
        pthread_cond_signal(&_consumerCond);

        pthread_mutex_unlock(&_mutex);
    }

    // 消费者调用
    void pop(T *out) // 输出型参数：*
    {
        pthread_mutex_lock(&_mutex);

        // 1、做判断
        if (isEmpty())
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
