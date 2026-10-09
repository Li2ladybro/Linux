#pragma once

#include "Log.hpp"

#include <cstdio>

#include <unistd.h>

#include <iostream>
#include <string>
#include <functional>


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
            outBuffer += " server[echo]";
            write(sock, outBuffer.c_str(), outBuffer.size());
        }
        else if (n == 0)
        {
            // 代表Client退出
            logMessage(NORMAL, "CLient Quit,Me Too!");
            break;
        }
    }
    close(sock);
}

// 计算任务
class Task
{
    // C++11 及以后语法，类型别名（using 别名），等价于老式 typedef ，但可读性更强。
    // 把复杂类型 X，起一个简短别名叫 func_t，后续直接写 func_t 就代表这个类型。
    using func_t = std::function<void(int)>;

    // typedef std::function<int(int,int)> func_t;
public:
    Task()
    {
    }
    Task(int sock, func_t func)
        : _sock(sock), _callBack(func)
    {
    }

    void operator()()
    {
        _callBack(_sock);
    }

private:
    int _sock;
    func_t _callBack;
};
