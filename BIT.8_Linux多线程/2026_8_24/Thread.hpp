#pragma once

#include <cstring>
#include <cassert>

#include <iostream>
#include <string>
#include <functional>

#include <pthread.h>

class Thread;

// 上下文
class Context
{
public:
    Context()
        : _this(nullptr), _argv(nullptr)
    {
    }
    ~Context()
    {
    }

public:
    Thread *_this;
    void *_argv;
};

class Thread
{
    typedef std::function<void *(void *)> func_t;
    const int num = 1024;

public:
    Thread(func_t func, void *argv, int number)
        : _func(func), _argv(argv)
    {
        char buffer[num];
        snprintf(buffer, sizeof buffer, "thread-%d", number);
        _name = buffer;

        Context *ctx = new Context;
        ctx->_this = this;
        ctx->_argv = _argv;

        int n = pthread_create(&_tid, nullptr, start_routine, ctx);
        // 意料之中用assert，意料之外用if
        assert(n == 0);
        // 在有些编译器下会有warning
        // 编译debug的方式发布的时候存在，release方式发布，assert就不存在了，n就是一个定义了，但是没有被使用的变量
        (void)n;
    }

    static void *start_routine(void *argv)
    {
        // 添加static消除this指针变量
        Context *ctx = (Context *)argv;
        void *ret = ctx->_this->run(ctx->_argv);
        delete ctx;
        return ret;
    }

    void join()
    {
        int n = pthread_join(_tid, nullptr);
        assert(n == 0);
    }

    void *run(void *argv)
    {
        return _func(_argv);
    }

    ~Thread()
    {
    }

private:
    std::string _name;
    pthread_t _tid;
    func_t _func;
    void *_argv;
};
