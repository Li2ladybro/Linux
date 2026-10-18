#pragma once

#include "protocol.hpp"

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

    class CalClient
    {
    public:
        CalClient(const string &serverip, const uint16_t &serverport)
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
                while (true)
                {
                    cout << "Please Enter# \n";
                    getline(cin, message); // 1 + 1

                    Request request(1, '+', 1);

                    // 对请求进行序列化
                    string request_serialize_str;
                    request.serialize(&request_serialize_str);
                    // std::cout<<request_serialize_str<<std::endl;
                    std::cout << "请求序列化#:\n"
                              << request_serialize_str;

                    std::string send_encrypted_str = EncryptText(request_serialize_str);
                    std::cout << "对请求序列化加密#\n"
                              << send_encrypted_str << std::endl;

                    // 发送加密的序列化数据
                    send(_sockfd, send_encrypted_str.c_str(), send_encrypted_str.size(), 0);

                    string inbuffer, reponse_package;
                    Response reponse;

                    // reponse_package -> "textlen/r/n_exit_code _result/r/n"
                    ReceivePackage(_sockfd, inbuffer, &reponse_package);
                    // 解包
                    string decrypted_package = DecryptText(reponse_package);
                    reponse.deserialize(decrypted_package);
                    cout << "exit_code：" << reponse._exit_code << "result：" << reponse._result << endl;
                }
            }
        }

        ~CalClient()
        {
            if (_sockfd >= 0)
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