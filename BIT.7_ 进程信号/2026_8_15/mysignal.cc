#include <iostream>
#include <unistd.h>
#include <signal.h>

using namespace std;

#define BLAOCK_SIGANL 2
#define MAX_SIIGNUM 31

void showpending(const sigset_t &pending)
{
    // for (auto &e : pending.__val)
    // {
    //     cout << e << endl;
    // }
    for (int i = 1; i < MAX_SIIGNUM; ++i)
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

int main()
{
    sigset_t block, oblock, pending;
    // 1.1、初始化
    sigemptyset(&block);
    sigemptyset(&oblock);

    // 1.2、添加需要屏蔽的信号
    sigaddset(&block, BLAOCK_SIGANL);

    // 1.3、开始屏蔽信号
    sigprocmask(SIG_SETMASK, &block, &oblock);

    // 2、遍历打印pending
    while (true)
    {
        // 2.1、初始化
        sigemptyset(&pending);
        // 2.2、获取
        sigpending(&pending);
        // 2.3、打印
        showpending(pending);
        sleep(1);
    }

    return 0;
}