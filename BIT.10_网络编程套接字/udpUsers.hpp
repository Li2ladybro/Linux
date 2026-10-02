#pragma once

#include <cstring> // <--- 必须加：提供 memset
#include <cstdint>

#include <sys/socket.h>
#include <arpa/inet.h> // <--- 必须加：提供 sockaddr_in, inet_addr, htons

#include <iostream>

#include <unordered_map>
#include <string>

using namespace std;

class User
{

public:
    User(const string &ip, const uint16_t &port)
        : _ip(ip), _port(port)
    {
    }
    ~User()
    {
    }

    const string &getUserIP()
    {
        return _ip;
    }

    const uint16_t &getUserPort()
    {
        return _port;
    }

private:
    string _ip;
    uint16_t _port;
};

class OnlineUsers
{

public:
    OnlineUsers()
    {
    }
    ~OnlineUsers()
    {
    }

    void addUser(const string &ip, const uint16_t port)
    {
        string k = ip + "-" + to_string(port);
        _onlineUsers.insert(make_pair(k, User(ip, port)));
    }

    void delUser(const string &ip, const uint16_t port)
    {
        string k = ip + "-" + to_string(port);
        _onlineUsers.erase(k);
    }

    bool isOnline(const string &ip, const uint16_t port)
    {
        string k = ip + "-" + to_string(port);

        unordered_map<string, User>::iterator it = _onlineUsers.find(k);
        if (it == _onlineUsers.end())
        {
            return false;
        }
        else
        {
            return true;
        }
    }

    void broadMessage(int sockfd, string clientip, uint16_t clientport, const string &message)
    {
        struct sockaddr_in tmp;

        for (auto &user : _onlineUsers)
        {

            memset(&tmp, 0, sizeof tmp);

            tmp.sin_family = AF_INET;
            tmp.sin_addr.s_addr = inet_addr(user.second.getUserIP().c_str());
            tmp.sin_port = htons(user.second.getUserPort());

            // 谁发的数据包头写谁的信息
            string s = clientip + "-" + to_string(clientport) + "# " + message;
            sendto(sockfd, s.c_str(), s.size(), 0, (struct sockaddr *)&tmp, sizeof tmp);
        }
    }

private:
    unordered_map<string, User> _onlineUsers;
};
