#include <cstdio>

#include <iostream>

#include <signal.h>

using namespace std;

void toZero(int num)
{
    while (num--)
    {
        printf("num:%2d\r", num);
        fflush(stdout);
        sleep(1);
    }
    cout << endl;
}

// void myHandler(int num)
// {
//     cout<<"get signum："<<num<<" pid is ："<<getpid()<<" 2号信号处理中..."<<endl;
//     toZero(10);
// }

int quit = 0;

void myHandler(int num)
{
    cout << num << " 号正在被递达" << " pid is ：" << getpid() << endl;
    cout << "quit：" << quit << "->";
    quit = 1;
    cout << quit << endl;
}

int main()
{
    /*
    struct sigaction act,oact;
    act.sa_flags=0;
    sigemptyset(&act.sa_mask);
    // 当我们正在处理某一种信号的时候，我们也想顺便屏蔽其他信号，就可以添加到这个sa_mask中，只有当该信号递达之后才解除所有阻塞
    sigaddset(&act.sa_mask,2);
    act.sa_handler=myHandler;

    sigaction(SIGINT,&act,&oact);
    while(true)
    {
        sleep(1);
    }
    */
    signal(SIGINT,myHandler);
    while (!quit)
        ;
    cout << "我正常退出\n";
    return 0;
}