#pragma once

#include "log.hpp"
#include <iostream>
#include <string>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <cerrno>
#include <cstring>

#include <sys/wait.h>
#include <unistd.h>
#include <strings.h>
#include <functional>

namespace Server
{
    using namespace std;
    static const uint16_t gport = 8080;
    static const int gbacklog = 5;

    enum
    {
        UASGE_ERR = 1,
        SOCKET_ERR,
        BIND_ERR,
        LISTEN_ERR
    };

    typedef function<void(int, string, uint16_t, string)> func_t;

    class TcpServer
    {
    public:
        TcpServer(const u_int16_t &port = gport)
            : _port(port), _listenSockfd(-1)
        {
        }

        void initServer()
        {
            // 1、创建套接字，得到一份缓冲区
            _listenSockfd = socket(AF_INET, SOCK_STREAM, 0);
            if (-1 == _listenSockfd)
            {
                logMessage(FATAL, "Create Socket Error");
                exit(SOCKET_ERR);
            }
            logMessage(NORMAL, "Create Socket Success _listenSockfd：" + to_string(_listenSockfd));
            // cout << "socket success" << ":" << _sockfd << endl;

            // 2、绑定套接字(port,ip)重要的是绑定port
            struct sockaddr_in local; // 定义了一个变量，属于用户栈

            bzero(&local, sizeof local);

            local.sin_family = AF_INET;
            // port和ip需要发给对方
            // 服务器需要明确端口号，不可以随意发生变化
            local.sin_port = htons(_port); // 主机转网络序列
            local.sin_addr.s_addr = INADDR_ANY;
            // local.sin_addr.s_addr = htonl(INADDR_ANY); // INADDR_ANY：0 可以接收任意地址的信息

            if (0 != bind(_listenSockfd, (struct sockaddr *)&local, sizeof local))
            {
                logMessage(FATAL, "Bind Socket Error");
                // cerr << "Bind Error " << errno << ": " << strerror(errno) << endl;
                exit(BIND_ERR);
            }

            logMessage(NORMAL, "Bind Socket Success");
            // cout << "Bind Socket Success\n";

            // 3、设置socket为监听状态(TCP是面向连接的)
            if (-1 == listen(_listenSockfd, gbacklog))
            {
                logMessage(FATAL, "Listen Socket Error");
                // cerr << "Listen Error ";
                exit(LISTEN_ERR);
            }
            logMessage(NORMAL, "Listen Socket Success");

            // UDP Server的预备工作完成
        }

        void start()
        {
            // 服务器的本质是一个常驻内存的进程死循环
            // 例如：操作系统

            for (;;)
            {
                signal(SIGCHLD, SIG_IGN);
                
                // 4、server 获取新连接
                // sock 是需要和 Client 进行通信的fd
                struct sockaddr_in peer;
                socklen_t len = sizeof peer;

                // 获取新链接
                int sock = accept(_listenSockfd, (struct sockaddr *)&peer, &len);

                if (sock == -1)
                {
                    logMessage(ERR0R, "Accept Error");
                    continue;
                }

                logMessage(NORMAL, "Accept A New Link Success");
                cout << sock << endl;

                // 5、这里就是一个 sock，未来通信就用这个 sock，Tcp面向字节流的，后续全部是文件（I/O操作）
                // 提供服务

                // demo1
                // serviceIO(sock);
                // close(sock);// 对于已经使用完毕的 fd 需要关闭，否则会导致文件描述符泄露

                // // demo2 多进程（1）
                // pid_t id = fork();
                // if (id == 0)
                // {
                //     // 子进程
                //     close(_listenSockfd);
                //     if (fork() > 0)
                //     {
                //         exit(0);
                //     }
                //     else
                //     {
                //         // 孤儿进程
                //         serviceIO(sock);
                //         close(sock);
                //         exit(0);
                //     }
                // }
                // // 父进程
                // pid_t ret = waitpid(id, nullptr, 0);
                // if (ret == id)
                // {
                //     cout << "Wait Success" << ret << endl;
                // }

                // demo2 多进程（2）
                pid_t id = fork();
                if (id == 0)
                {
                    // 子进程
                    close(_listenSockfd);
                    serviceIO(sock);
                    close(sock);
                    exit(0);
                }
                close(sock);
            }
        }

        void serviceIO(int sock)
        {
            char buffer[1024];
            while (true)
            {
                ssize_t n = read(sock, buffer, sizeof buffer - 1);
                if (n > 0)
                {
                    buffer[n] = 0;
                    cout << "Receive Message# " << buffer << endl;
                    string outBuffer = buffer;
                    outBuffer += "server[echo]";
                    write(sock, outBuffer.c_str(), outBuffer.size());
                }
                else if (n == 0)
                {
                    // 代表Client退出
                    logMessage(NORMAL, "CLient Quit,Me Too!");
                    break;
                }
            }
        }

        ~TcpServer()
        {
        }

    private:
        // 标识唯一的进程
        uint16_t _port;

        int _listenSockfd; // 不是用来通信的，只负责监听是否有新连接到来，并获取，相当于拉客的

        func_t _callback; // 收到数据怎么处理
    };
}