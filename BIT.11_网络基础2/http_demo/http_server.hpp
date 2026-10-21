#pragma once

#include "protocol.hpp"

#include <cerrno>
#include <cstring>

#include <iostream>
#include <string>
#include <functional>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <sys/wait.h>
#include <unistd.h>
#include <strings.h>

namespace Server
{

    enum
    {
        UASGE_ERR = 1,
        SOCKET_ERR,
        BIND_ERR,
        LISTEN_ERR
    };

    static const int gbacklog = 5;

    // const Request &：输入型，Response &：输出型
    // typedef function<bool(const HttpRequest &req, HttpResponse &resp)> func_t;

    using func_t = std::function<bool(HttpRequest &req, HttpResponse &resp)>;

    // 保证解耦
    void HandleEnter(int sock, func_t func)
    {
        // 1、获取请求
        // 2、解包，反序列化
        // 3、应用层业务逻辑处理
        // 4、将响序列化
        // 5、加密，回复响应
        char inbuffer[4096];
        HttpRequest req;
        HttpResponse resp;

        ssize_t n = recv(sock, inbuffer, sizeof inbuffer - 1, 0);
        if (n > 0)
        {
            inbuffer[n] = 0;
            req.inbufer = inbuffer;
            func(req, resp);
            send(sock, resp.outbuffer.c_str(), resp.outbuffer.size(), 0);
        }
    }

    class HttpServer
    {
    public:
        HttpServer(const uint16_t port, func_t fun)
            : _port(port), _listenSockfd(-1), _func(fun)
        {
        }

        void initServer()
        {
            // 1、创建套接字，得到一份缓冲区
            _listenSockfd = socket(AF_INET, SOCK_STREAM, 0);
            if (-1 == _listenSockfd)
            {
                exit(SOCKET_ERR);
            }

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
                // cerr << "Bind Error " << errno << ": " << strerror(errno) << endl;
                exit(BIND_ERR);
            }

            // 3、设置socket为监听状态(TCP是面向连接的)
            if (-1 == listen(_listenSockfd, gbacklog))
            {
                exit(LISTEN_ERR);
            }
            // UDP Server的预备工作完成
        }

        void start()
        {
            // 服务器的本质是一个常驻内存的进程死循环
            // 例如：操作系统
            for (;;)
            {

                // 4、server 获取新连接
                // sock 是需要和 Client 进行通信的fd
                struct sockaddr_in peer;
                socklen_t len = sizeof peer;

                // 获取新链接
                int sock = accept(_listenSockfd, (struct sockaddr *)&peer, &len);

                if (sock == -1)
                {
                    continue;
                }

                // 会话层

                // demo2 多进程（2），让孤儿进程去处理
                pid_t id = fork();
                if (id == 0)
                {
                    // 子进程
                    close(_listenSockfd);
                    if (fork() > 0)
                    {
                        exit(0);
                    }
                    // 表示层
                    HandleEnter(sock, _func);
                    close(sock);
                    exit(0);
                }

                close(sock);
                pid_t ret = waitpid(id, nullptr, 0);
            }
        }

        ~HttpServer()
        {
        }

    private:
        // 标识唯一的进程
        uint16_t _port;

        int _listenSockfd; // 不是用来通信的，只负责监听是否有新连接到来，并获取，相当于拉客的

        func_t _func;
    };
}