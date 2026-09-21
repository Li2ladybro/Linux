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
#include <functional>

namespace Server
{
    using namespace std;
    static const int gnum = 1024;
    enum
    {
        UASGE_ERR = 1,
        SOCKET_ERR,
        BIND_ERR
    };

    typedef function<void(string, uint16_t, string)> func_t;

    class udpServer
    {
    public:
        udpServer(const func_t &callback, const u_int16_t &port, const string &ip = _defaultIp)
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
            cout << "socket success" << ":" << _sockfd << endl;

            // 2、绑定套接字(port,ip)重要的是绑定port
            struct sockaddr_in local; // 定义了一个变量，属于用户栈

            bzero(&local, sizeof local);

            local.sin_family = AF_INET;
            // port和ip需要发给对方
            // 服务器需要明确端口号，不可以随意发生变化
            local.sin_port = htons(_port);
            local.sin_addr.s_addr = inet_addr(_ip.c_str()); // ip:1、string->uint32_t 2、htonl()
            // local.sin_addr.s_addr = htonl(INADDR_ANY); // INADDR_ANY：0 可以接收任意地址的信息
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

            char buffer[gnum];
            for (;;)
            {
                struct sockaddr_in peer;
                socklen_t len = sizeof peer;

                ssize_t s = recvfrom(_sockfd, buffer, sizeof buffer - 1,
                                     0, (sockaddr *)&peer, &len);

                // 1、数据是什么。2、谁发的
                if (s > 0)
                {
                    buffer[s] = 0;
                    string clientip = inet_ntoa(peer.sin_addr); // 1、网络序列 2、int->点分十进制
                    uint16_t clientport = ntohs(peer.sin_port);
                    string message = buffer;

                    cout << clientip << "[" << clientport << "]# " << message << endl;
                    // 读上来就结束了吗
                    _callback(clientip, clientport, message);
                }
                sleep(1);
            }
        }

        ~udpServer()
        {
        }

    private:
        inline static const string _defaultIp = "0.0.0.0"; // 默认Ip

        // 标识唯一的进程
        uint16_t _port;
        string _ip; // 实际上，一款网络服务器，不建议指明一个ip

        int _sockfd; // socket文件描述符

        func_t _callback; // 收到数据怎么处理
    };
}