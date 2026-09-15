#pragma once

#include <cstdio>

#include <iostream>
#include <string>
#include <functional>

// 计算任务
class Task
{
    // C++11 及以后语法，类型别名（using 别名），等价于老式 typedef ，但可读性更强。
    // 把复杂类型 X，起一个简短别名叫 func_t，后续直接写 func_t 就代表这个类型。
    using func_t = std::function<int(int, int, char)>;

    // typedef std::function<int(int,int)> func_t;
public:
    Task()
    {
    }
    Task(int x, int y, char op, func_t func)
        : _x(x), _y(y), _op(op), _callBack(func)
    {
    }

    std::string operator()()
    {
        char buffer[64];
        snprintf(buffer, sizeof buffer, "%d %c %d = %d ", _x, _op, _y, _callBack(_x, _y, _op));
        return buffer;
    }

    std::string toTaskString()
    {
        char buffer[64];
        snprintf(buffer, sizeof buffer, "%d %c %d = ? ", _x, _op, _y);
        return buffer;
    }

private:
    int _x;
    int _y;
    char _op;
    func_t _callBack;
};

const std::string oper = "+-*/%";

int myop(int x, int y, char op)
{
    switch (op)
    {
    case '+':
        return x + y;

    case '-':
        return x - y;

    case '*':
        return x * y;

    case '/':
    {
        if (y == 0)
        {
            std::cerr << "div zero error!\n";
            return -1;
        }
        return x / y;
    }
    case '%':
        if (y == 0)
        {
            std::cerr << "mod zero error!\n";
            return -1;
        }
        return x % y;

    default:
        break;
    }
    return -1;
}