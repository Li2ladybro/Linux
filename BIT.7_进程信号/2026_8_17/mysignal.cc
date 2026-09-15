// #include <cstdio>

// #include <iostream>

// #include <signal.h>

// using namespace std;

// void toZero(int num)
// {
//     while (num--)
//     {
//         printf("num:%2d\r", num);
//         fflush(stdout);
//         sleep(1);
//     }
//     cout << endl;
// }

// // void myHandler(int num)
// // {
// //     cout<<"get signum："<<num<<" pid is ："<<getpid()<<" 2号信号处理中..."<<endl;
// //     toZero(10);
// // }

// int quit = 0;

// void myHandler(int num)
// {
//     cout << num << " 号正在被递达" << " pid is ：" << getpid() << endl;
//     cout << "quit：" << quit << "->";
//     quit = 1;
//     cout << quit << endl;
// }

// int main()
// {
//     /*
//     struct sigaction act,oact;
//     act.sa_flags=0;
//     sigemptyset(&act.sa_mask);
//     // 当我们正在处理某一种信号的时候，我们也想顺便屏蔽其他信号，就可以添加到这个sa_mask中，只有当该信号递达之后才解除所有阻塞
//     sigaddset(&act.sa_mask,2);
//     act.sa_handler=myHandler;

//     sigaction(SIGINT,&act,&oact);
//     while(true)
//     {
//         sleep(1);
//     }
//     */
//     signal(SIGINT,myHandler);
//     while (!quit)
//         ;
//     cout << "我正常退出\n";
//     return 0;
// }

#include <iostream>

#include <signal.h>

using namespace std;

void myHandler(int num)
{
    // 1.我有非常多的子进程，在同一个时刻退出了
    // while (1)
    // {
    //     waitpid(-1);
    // }
    // 2.我有非常多的子进程，在同一个时刻只有一部分退出了
    // // 用 while 循环 + WNOHANG，一次性回收多个已退出的子进程
    // 防止多个子进程同时退出时信号合并导致漏回收
    // while (waitpid(-1, NULL, WNOHANG) > 0)
    // {
    //     printf("reaped a child\n");
    // }
    // 非阻塞式等待，如果返回零表示当前没进程退出
    // if (ret == 0)
    //     break;

    cout << "正在递达子进程退出信号 pid is " << getpid() << endl;
}

int main()
{
    // 显示的对子进程退出信号忽略
    signal(SIGCHLD, SIG_IGN);
    pid_t id = fork();
    if (id == 0)
    {
        int i = 2;
        while (i)
        {
            cout << "我是子进程我要退出来了，pid is " << getpid() << " toZero " << i-- << endl;

            sleep(1);
        }
        exit(-1);
    }

    int i = 5;
    while (i)
    {
        cout << "我是父进程我要退出来了，pid is " << getpid() << " toZero " << i-- << endl;
        sleep(1);
    }

    return 0;
}