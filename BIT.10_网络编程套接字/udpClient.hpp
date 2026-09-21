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

namespace Client
{
    using namespace std;
    static const int gnum = 1024;
    enum
    {
        UASGE_ERR = 1,
        SOCKET_ERR,
        BIND_ERR
    };

    class udpClient
    {
    public:
        udpClient(const string &serverip, const uint16_t &serverport)
            : _serverip(serverip), _serverport(serverport), _sockfd(-1), _quit(false)
        {
        }

        void initClient()
        {
            // 1、创建套接字，得到一份缓冲区
            _sockfd = socket(AF_INET, SOCK_DGRAM, 0);
            if (-1 == _sockfd)
            {
                cerr << "Socket Error: " << errno << ": " << strerror(errno) << endl;
                exit(SOCKET_ERR);
            }

            // 2、客户端需不需要bind[必须要]，客户端不需要显示bind，由os绑
            // 写服务器的是一家公司。写客户端的是无数家 -- OS在什么时候，如何bind
            cout << "socket success" << ":" << _sockfd << endl;
        }

        void run()
        {
            // 目标地址
            struct sockaddr_in server;
            memset(&server, 0, sizeof server);

            server.sin_family = AF_INET;
            server.sin_addr.s_addr = inet_addr(_serverip.c_str());
            server.sin_port = htons(_serverport);

            string messages;
            while (!_quit)
            {
                cout << "Please Enter# ";
                cin >> messages;

                // 首次发送数据的时候绑定
                sendto(_sockfd, messages.c_str(), messages.size(), 0, (struct sockaddr *)&server, sizeof server);
            }
        }

        ~udpClient()
        {
        }

    private:
        // 标识唯一的进程
        string _serverip;     // 目标服务器ip
        uint16_t _serverport; // 目标服务器端口

        bool _quit;
        int _sockfd; // socket文件描述符
    };
}