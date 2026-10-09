#pragma once

#include <unistd.h>
#include <signal.h>
#include <cstdlib>
#include <cassert>
#include <fcntl.h>

#define DEV "/dev/null" // 文件黑洞，吸收输出输入

// 守护进程化
void DaemonSelf(const char *currPath = nullptr)
{
    // 1、让调用进程忽略异常信号
    signal(SIGPIPE, SIG_IGN);

    // 2、如何让自己不是组长 setsid
    if (fork() > 0)
    {
        exit(0);
    }
    // 子进程---守护进程，精灵进程，本质就是孤儿进程的一种！
    // The  calling process also becomes the process group leader of a new process group in the session
    pid_t n = setsid();
    assert(n != -1);

    // 3、守护进程是脱离终端的，关闭或者重定向以前进程默认打开的文件
    // 该后台进程不关心键盘和屏幕流
    int fd = open(DEV, O_RDWR);
    if (fd != -1)
    {
        // 重定向
        dup2(fd, 0);
        dup2(fd, 1);
        dup2(fd, 2);
        close(fd);
    }
    else
    {
        close(0);
        close(1);
        close(2);
    }
    if (currPath)
    {
        chdir(currPath);
    }
}