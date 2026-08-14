#include <cstdlib>

#include <iostream>
#include <string>

#include <unistd.h>
#include <signal.h>

using namespace std;

int num = 1;

void handler(int no)
{
    cout << "我捕捉到了" << num << "号信号\n";
}

void Usage(string proc)
{
    cout << "Usage：" << proc << " pid signo\n";
}

int main(int argc, char *argv[])
{
    /*
    if (argc != 3)
    {
        Usage(argv[0]);
        exit(1);
    }

    pid_t pid = atoi(argv[1]);
    int signo = atoi(argv[2]);
    if (kill(pid, signo) != 0)
    {
        perror("kill:");
    }

    这里是signal函数的调用，并不是handler的调用
    仅仅是设置了对2号信号的捕捉方法，并不代表该方法被调用了
    一般这个方法不会执行，除非收到对应的信号!

    signal(2, handler);

    while (true)
    {
        cout << "hello i am " << getpid() << endl;
        sleep(1);
    }
    */

    /*
    2、系统调用向目标进程发送信号
    kill()可以想任意进程发送任意信号
    raise()给自己发送任意信号kill(getpid(),任意信号)
    abort()给自己发送指定的信号SIGABRT,kil1(getpid(),SIGABRT)

    int cnu = 0;
    while (cnu < 10)
    {
        cout << cnu++ << ":" << endl;
        if (cnu > 5)
        {
            // The  raise()  function  sends  a  signal to the calling process or thread.
            abort();  // kill(getpid(), SIGABRT)
            raise(9); // kill(getpid(), signo)
        }
    }
    */

    /*
    // 3.产生信号的方式:硬件异常产生信号信号产生，不一定非得用户显示的发送!
    signal(SIGFPE, handler);
    int a = 10;
    // 如何证明?
    // 受到信号，不一定会引起进程退出 --没有退出，有可能还会被调到
    // --CPU内部的寄存器只有一份，但是寄存器中的内容，属于当前进程的上下文!
    // 你有没有能力或者动作修正这个问题呢?->没有
    // 当进程被切换的时候，就有无数次状态寄存器被保存和回复的过程
    // 所以每一次恢复的时候，就让OS识别到了CPU内部的溢出状态寄存器中的溢出标志位是1
    // OS如何得知应该给当前进程发送8号信号的--OS怎么知道我除0了呢??，CPU会异常
    a/= 0;//为什么除0会终止进程?当前进程会受到来自OS系统的信号(告知)，SIGFPE

    */
    alarm(1);
    /*
    while (true)
    {
        cout << "num:" << num++ << endl;
    }
    */

    signal(SIGALRM, handler);
    alarm(1);
    while (num++)
        ;
    return 0;
}