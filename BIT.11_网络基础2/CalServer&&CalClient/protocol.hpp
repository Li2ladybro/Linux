#pragma once

#include <cstring>

#include <iostream>
#include <string>

enum
{
    OK = 0,
    DIVZERO,
    MODZERO,
    OPERROR
};

#define SEP " "
#define SEP_LEN strlen(SEP)

#define LINE_SEP "\r\n"
#define LINE_SEP_LEN strlen(LINE_SEP)

// 加密
string EncryptText(const string &text)
{
}

// 解密
string DecryptText(const string &package)
{
}

class Request
{
public:
    Request()
        : _x(0),
          _op(0), _y(0)
    {
    }

    Request(int x, char op, int y)
        : _x(x), _op(op), _y(y)
    {
    }

    // 序列化
    bool serialize(string *out)
    {
        *out = "";
        // Request 结构化 -》"x op y"
        string x_string = to_string(_x);
        string y_string = to_string(_y);

        *out = x_string;
        *out += SEP;
        *out += _op;
        *out += SEP;
        *out += y_string;
        *out += LINE_SEP;

        return out;
    }

    // 反序列化
    bool deserialize(const string &in)
    {
        // "x op y" -》结构化
        auto left = in.find(SEP);
        auto right = in.rfind(SEP);

        if (left == string::npos || string::npos == right)
        {
            return false;
        }

        if (left == right)
        {
            return false;
        }
        string x_string = in.substr(0, left); // [0,2) [start,end) ，start，end - start
        _op = in[left + SEP_LEN];
        string y_string = in.substr(right + SEP_LEN);

        if (x_string.empty() || y_string.empty())
        {
            return false;
        }

        _x = stoi(x_string);
        _y = stoi(y_string);

        return true;
    }

    // " x op y"
    // "12 + 23"
    int _x;
    char _op;
    int _y;
};
class Response
{
public:
    Response()
        : _exit_code(0), _result(0)
    {
    }

    // 序列化
    bool serialize(string *out)

    {
    }
    // 反序列化
    bool deserialize()
    {
    }

    int _exit_code; // 0 计算成功，！0表示计算失败
    int _result;    // 最终的计算结果
};
