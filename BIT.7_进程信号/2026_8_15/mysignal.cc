#include <iostream>
#include <vector>

#include <unistd.h>
#include <signal.h>

using namespace std;

// #define BLAOCK_SIGANL 2
#define MAX_SIIGNUM 31

int arr[2] = {2, 3};

void showpending(const sigset_t &pending)
{
    // for (auto &e : pending.__val)
    // {
    //     cout << e << endl;
    // }
    for (int i = MAX_SIIGNUM; i > 0; --i)
    {
        if (sigismember(&pending, i))
        {
            cout << 1;
        }
        else
        {
            cout << 0;
        }
    }
    cout << endl;
}

void myHander(int sigNum)
{
    cout << "我捕捉到了" << sigNum << "信号信号并递达\n";
}

int main()
{
    sigset_t block, oblock, pending;
    // 1.1、初始化
    sigemptyset(&block);
    sigemptyset(&oblock);

    // 1.2、添加需要屏蔽的信号
    for (auto &e : arr)
    {
        sigaddset(&block, e);
        signal(e, myHander);
    }

    // 1.3、开始屏蔽信号
    sigprocmask(SIG_SETMASK, &block, &oblock);

    int cun = 10;
    // 2、遍历打印pending
    while (true)
    {
        // 2.1、初始化
        sigemptyset(&pending);
        // 2.2、获取
        // 内核把内核态的未决信号集复制到你的 pending 变量，直接覆盖。
        sigpending(&pending);
        // 2.3、打印
        showpending(pending);
        sleep(1);
        if (cun-- == 0)
        {
            cout << "恢复对信号的屏蔽，不去屏蔽任何信号\n";
            sigprocmask(SIG_SETMASK, &oblock, &block);
        }
    }

    return 0;
}