#pragma once

#include <cassert>

#include <string>
#include <functional>

#include <pthread.h>

namespace ThreadNs
{

    typedef std::function<void *(void *)> func_t;
    const int num = 1024;

    class Thread
    {
    private:
        static void *start_routine(void *argv)
        {
            Thread *thread = (Thread *)argv;
            return thread->callback();
        }

    public:
        static int _threadNum;

        Thread()
        {
            char nameBuffer[num];
            snprintf(nameBuffer, sizeof nameBuffer, "thread-%d", _threadNum++);
            _name = nameBuffer;
        }

        void start(func_t func, void *argv = nullptr)
        {
            _func = func;
            _argv = argv;
            int n = pthread_create(&_tid, nullptr, start_routine, (void *)this);
            // 意料之中用assert，意料之外用if
            assert(n == 0);
            // 在有些编译器下会有warning
            // 编译debug的方式发布的时候存在，release方式发布，assert就不存在了，n就是一个定义了，但是没有被使用的变量
            (void)n;
        }

        void join()
        {
            int n = pthread_join(_tid, nullptr);
            assert(n == 0);
        }

        std::string threadName()
        {
            return _name;
        }

        void *callback()
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

    int Thread::_threadNum = 1;

}
