// #include <iostream>

// #include <pthread.h>
// #include <unistd.h>

// using namespace std;

// int g_l = 0;

// void *start_routine(void *argv)
// {
//     int c = 0;

//     int *p = nullptr;
//     // *p = 0;
//     // while (c <= 10)
//     while (1)
//     {

//         cout /*<< (const char *)argv */ << " g_l : " << g_l++ << " start_routine\n";
//         c++;
//         // sleep(1);
//     }
// }

// int main()
// {
//     pthread_t tid;
//     int n = pthread_create(&tid, nullptr, start_routine, (void *)"吃了吗");
//     if (n != 0)
//     {
//         cout << "线程创建失败\n";
//         exit(-1);
//     }
//     int c = 0;
//     // while (c <= 10)
//     while (1)
//     {

//         char tidBuffer[64];
//         int n = snprintf(tidBuffer, sizeof tidBuffer, "0x%x", tid);
//         cout << /*"tid " << tidBuffer << " n " << n << */ " g_l : " << g_l++ << " main\n";
//         c++;
//         // sleep(1);
//     }

//     // cout << "hifh\n";
//     return 0;
// }

#include <cassert>

#include <iostream>
// #include <pthread>
#include <vector>

#include <unistd.h>
#include <pthread.h>

using namespace std;

class ThreadReturn
{
public:
    int exit_code;
    int exit_result;
};

class ThreadData
{
public:
    pthread_t tid;
    char name[64];
};

// std::thread t1();
// 1、可重入状态
// 2、是可重入函数
// 3、函数内定义的变量，都是独立的-》每一个线程都有自己的独立的栈结构
void *start_routine(void *argv)
{
    // 传入参数后才可以开启线程
    int cnt = 0;

    // exit()用来控制进程，任何一个线程调用会直接终止所有线程
    // exit(-1);

    while (cnt < 10)
    {
        cout << "I am thread " << ((ThreadData *)argv)->name << " cnt : " << cnt++ << endl;
        sleep(1);
        // ThreadReturn *ret = new ThreadReturn;
        // ret->exit_code = 2026;
        // ret->exit_result = 22;
        // pthread_exit((void *)ret);
        return (void *)23;
        // return (void *)ret;
    }

    // pthread_exit(nullptr);

    // delete (ThreadData *)argv;
    // 线程退出
    // 1、pthread_exit(nullptr);

    // 2、 return nullptr;
}

int main()
{
    vector<ThreadData *> threads;
#define NUM 10
    // 创建一批进程
    for (int i = 0; i < NUM; ++i)
    {
        ThreadData *thd = new ThreadData;
        snprintf(thd->name, sizeof(thd->name), "%s : %d", "thread", i);
        pthread_create(&(thd->tid), nullptr, start_routine, (void *)thd);
        threads.push_back(thd);
        /* code */
        // char nameBuffer[64];
        // snprintf(nameBuffer, sizeof nameBuffer, "%s : %d", "thread", i);
        // pthread_t tid;
        // pthread_create(&tid, nullptr, start_routine, (void *)nameBuffer);
    }

    for (auto &e : threads)
    {
        cout << "create thread " << e->name << " : " << e->tid << " success " << endl;
    }

    for (int i = 0; i < threads.size() / 2; ++i)
    {
        // 线程可以被取消，注意：线程取消的前提是线程已经跑起来了
        // 线程如果被取消，返回(void*)-1
        pthread_cancel(threads[i]->tid);
        cout << "pthread_cancel : " << threads[i]->name << " success \n";
    }

    for (auto &e : threads)
    {
        void *ret = nullptr;
        // pthread_join默认就会调用成功，是线程的等待函数，不需要接收异常信号，信号是进程该考虑的
        int n = pthread_join(e->tid, &ret);
        assert(n == 0);
        // cout << " join " << e->name << " success " << " exit_code : " << ((ThreadReturn *)ret)->exit_code << " exit_result : " << ((ThreadReturn *)ret)->exit_result << endl;
        cout << " join " << e->name << " success " << " exit_code : " << (long long)ret << endl;
        // delete (ThreadReturn *)ret;
        delete e;
    }

    cout << "main quit\n";
    // while (1)
    // {
    //     cout << "I am main thread\n";
    //     sleep(1);
    // }

    return 0;
}
