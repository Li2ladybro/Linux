#pragma once

#include <iostream>
#include <string>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include <cerrno>
#include <cstring>
#include <unistd.h>
#include <strings.h>

namespace Server
{
    using namespace std;
    class udpServer
    {
        enum
        {
            UASGE_ERR = 1,
            SOCKET_ERR,
            BIND_ERR
        };

    public:
        udpServer(const u_int16_t &port, const string &ip = _defaultIp)
            : _port(port), _ip(ip), _sockfd(-1)
        {
        }

        void initServer()
        {
            // 1、创建套接字，得到一份缓冲区
            _sockfd = socket(AF_INET, SOCK_DGRAM, 0);
            if (-1 == _sockfd)
            {
                cerr << "Socket Error: " << errno << ": " << strerror(errno) << endl;
                exit(SOCKET_ERR);
            }
            // 2、绑定套接字(port,ip)
            struct sockaddr_in local; // 定义了一个变量，用户栈

            bzero(&local, sizeof local);

            local.sin_family = AF_INET;
            // port和ip需要发给对方
            local.sin_port = htons(_port);
            local.sin_addr.s_addr = inet_addr(_ip.c_str()); // ip:1、string->uint32_t 2、htonl()

            if (0 != bind(_sockfd, (struct sockaddr *)&local, sizeof local))
            {
                cerr << "Bind Error " << errno << ": " << strerror(errno) << endl;
                exit(BIND_ERR);
            }
            
            // UDP Server的预备工作完成
        }

        void start()
        {
            // 服务器的本质是一个常驻内存的进程死循环
            // 例如：操作系统
            for (;;)
            {
                sleep(1);
            }
        }

        ~udpServer()
        {
        }

    private:
        inline const static string _defaultIp = "0.0.0.0"; // 默认Ip

        // 标识唯一的进程
        uint16_t _port;
        string _ip;

        int _sockfd; // socket文件描述符
    };
}