// #include <iostream>
// #include <cstdio>
// #include <cstring>
// #include <cassert>
// #include <unistd.h>
// #include <sys/wait.h>

// using namespace std;

// int main()
// {
//     int fds[2];
//     assert(pipe(fds) == 0);
//     // 谁是读写？
//     // [0] 是读端，嘴巴是用来读的
//     // [1] 是写端，钢笔是用来写的
//     // cout<<"fds[0] = "<<fds[0]<<endl;
//     // cout<<"fds[1] = "<<fds[1]<<endl;
//     pid_t pid = fork();
//     assert(pid != -1);
//     if (pid == 0)
//     {
//         // 子进程进行写入
//         close(fds[0]); // 关闭读端
//         // 子进程通信代码
//         const char *s = "我是子进程，我在给你信息";
//         int cnt = 0;
//         while (true)
//         {
//             char buf[1024];
//             snprintf(buf, sizeof buf, "child->paren say:%s[%d][%d]", s, cnt++, getpid());
//             // 如果管道被写满，则写进程阻塞，等待读进程读取管道数据
//             write(fds[1], buf, strlen(buf));
//             cout << "管道内数据大小：" << cnt * strlen(buf) << "Byte" << endl;
//             sleep(1);     // 每隔一秒写一次
//             // break;
//         }

//         close(fds[1]);
//         cout << "子进程关闭了自己的写端\n";
//         sleep(1);
//         exit(0);
//     }
//     // 父进程进行读取
//     close(fds[1]); // 关闭写端
//     // 父进程通信代码
//     while (true)
//     {
//         sleep(100000);
//         char buf[1024];
//         // cout << "AAAAAAAAAAAAAAA\n";
//         // 如果管道内无数据则读进程阻塞，等待写进程写入数据
//         ssize_t s = read(fds[0], buf, sizeof(buf) - 1); // 每次读走1023个字节
//         // cout << "BBBBBBBBBBBBBBBB\n";
//         if (s > 0)
//         {
//             buf[s] = '\0';
//             cout << "Get Message #" << buf << " | my pid ：" << getpid() << endl;
//         }
//         else
//         {
//             // 读到文件结尾
//             cout << "read" << s << endl;
//             break;
//         }
//         break; // 读一次不读了
//         // 父进程没有sleep一直在读
//     }
//     close(fds[0]);
//     cout << "父进程不想读了，关闭了读端\n";
//     int status = 0;
//     int ret = waitpid(pid, &status, 0);
//     assert(pid == ret);
//     // cout<<"ncndjcdncjdcdcQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ\n";
//     cout << "pid->" << ret << ":" << (status & 0x7f) << endl;
//     // close(fds[0]);
//     return 0;
// }


#include <iostream>
#include <unistd.h>
#include <cassert>
#include <cstring>
#include <signal.h>

int main()
{
    int fds[2];
    int ret = pipe(fds);
    assert(ret == 0);

    // fds[0]读端，fds[1]写端
    int rfd = fds[0];
    int wfd = fds[1];

    // 关键：读端保持打开，但是不去read！！让数据堆积在内核管道缓冲区
    // close(rfd); // 千万不能关！关了写就SIGPIPE炸掉

    char block[1024];
    memset(block, 'A', sizeof(block));

    size_t total = 0;
    while(true)
    {
        // 写1024字节一块
        ssize_t w = write(wfd, block, sizeof(block));
        if(w == -1)
        {
            perror("write");
            break;
        }
        total += w;
        std::cout << "已写入: " << total << " 字节" << std::endl;
    }

    close(rfd);
    close(wfd);
    return 0;
}