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

    class TcpClient
    {
    public:
        TcpClient(const string &serverip, const uint16_t &serverport)
            : _serverip(serverip), _serverport(serverport), _sockfd(-1)
        {
        }

        void initClient()
        {
            // 1、创建套接字，得到一份缓冲区
            _sockfd = socket(AF_INET, SOCK_STREAM, 0);
            if (-1 == _sockfd)
            {
                cout << " Socket Create Error\n";
                // cerr << "Socket Error: " << errno << ": " << strerror(errno) << endl;
                exit(SOCKET_ERR);
            }

            // 2、Tcp客户端需不需要bind[必须要]，客户端不需要显示bind，由os绑
            // 写服务器的是一家公司。写客户端的是无数家 -- OS在什么时候，如何bind
            cout << "socket success" << "：" << _sockfd << endl;
            // 3、要不要 listen?不要
            // 4、要不要 accept?不要
            // 5、要什么呢？？发起链接
        }

        void start()
        {
            // pthread_create(&_read, nullptr, readServerResponse, (void *)&_sockfd);
            // 目标地址
            struct sockaddr_in server;
            memset(&server, 0, sizeof server);

            server.sin_family = AF_INET;
            server.sin_addr.s_addr = inet_addr(_serverip.c_str());
            server.sin_port = htons(_serverport);

            if (-1 == connect(_sockfd, (struct sockaddr *)&server, sizeof server))
            {
                cout << " Socket Connect Error\n";
            }
            else
            {
                string message;
                char buffer[1024];
                cout<<"Client _sockfd: "+to_string(_sockfd)<<endl;
                while (true)
                {
                    cout << "Please Enter# ";
                    getline(cin, message);

                    write(_sockfd, message.c_str(), message.size());

                    int n = read(_sockfd, buffer, sizeof buffer - 1);
                    if (n > 0)
                    {
                        buffer[n] = 0;
                        cout << "Server 回显# " << buffer << endl;
                    }
                    else
                    {
                        break;
                    }
                }
            }
        }

        ~TcpClient()
        {
            if(_sockfd>=0)
            {
                close(_sockfd);
            }
        }

    private:
        // 标识唯一的进程
        string _serverip;     // 目标服务器ip
        uint16_t _serverport; // 目标服务器端口
        int _sockfd;          // socket文件描述符
    };
}