#pragma once

#include "log.hpp"
#include "protocol.hpp"

#include <iostream>
#include <string>
#include <functional>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <cerrno>
#include <cstring>

#include <sys/wait.h>
#include <unistd.h>
#include <strings.h>

namespace Server
{

    using namespace std;

    enum
    {
        UASGE_ERR = 1,
        SOCKET_ERR,
        BIND_ERR,
        LISTEN_ERR
    };

    static const uint16_t gport = 8080;
    static const int gbacklog = 5;

    // const Request &：输入型，Response &：输出型
    typedef function<bool(const Request &req, Response &resp)> func_t;

    // 保证解耦
    void HandleEnter(int sock, func_t func)
    {
        string inbuffer;

        while (true)
        {
            // 1、读取："textlen/r/nx op y/r/n"
            // 1.1、你怎麽保证你读到的消息是【一个】完整的请求，这个请求是序列化的字符串
            string encrypted_text;
            if (ReceivePackage(sock, inbuffer, &encrypted_text) == false)
            {
                // 未收到数据
                return;
            }
            std::cout << "带包头的报文: \n"
                      << encrypted_text << std::endl;
            // req_text -> "textlen/r/nx op y/r/n"

            // 1.2、解密
            // "x op y"
            string decrypted_str = DecryptText(encrypted_text);
            std::cout << "去掉包头的报文: \n"
                      << decrypted_str << std::endl;

            // 2、反序列化
            // 2.1、得到一个结构化的请求对象
            Request structured_req;

            if (structured_req.deserialize(decrypted_str) == false)
            {
                return;
            }

            // 3、计算机处理，这块是真的业务逻辑
            // 3.1、得到一个结构化的响应
            Response structured_resp;
            func(structured_req, structured_resp);

            // 4、对响应 Response，进行序列化
            // 4.1、得到一个字符串
            string serialize_resp_str;
            structured_resp.serialize(&serialize_resp_str);

            std::cout << "计算完成的序列化响应: \n"
                      << serialize_resp_str << std::endl;

            // 5、发送响应结果
            // 对 text 加密
            string send_encrypted_str = EncryptText(serialize_resp_str);

            std::cout << "构建完成的完整响应: \n"
                      << send_encrypted_str << std::endl;

            send(sock, send_encrypted_str.c_str(), send_encrypted_str.size(), 0);
        }
    }

    class CalServer
    {
    public:
        CalServer(const u_int16_t &port = gport)
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
            logMessage(NORMAL, "Create Socket Success _listenSockfd: %d", _listenSockfd);
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

        void start(func_t func)
        {
            // 服务器的本质是一个常驻内存的进程死循环
            // 例如：操作系统
            for (;;)
            {
                // signal(SIGCHLD, SIG_IGN);

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

                logMessage(NORMAL, "Accept A New Link Success Get A New Sock : %d", sock);

                // demo2 多进程（2）
                pid_t id = fork();
                if (id == 0)
                {
                    // 子进程
                    close(_listenSockfd);
                    HandleEnter(sock, func);
                    close(sock);
                    exit(0);
                }

                close(sock);
                pid_t ret = waitpid(id, nullptr, 0);
                if (ret > 0)
                {
                    logMessage(NORMAL, "Wait Success");
                }
            }
        }

        ~CalServer()
        {
        }

    private:
        // 标识唯一的进程
        uint16_t _port;

        int _listenSockfd; // 不是用来通信的，只负责监听是否有新连接到来，并获取，相当于拉客的
    };
}