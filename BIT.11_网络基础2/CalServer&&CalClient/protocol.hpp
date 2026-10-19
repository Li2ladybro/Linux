#pragma once

#include <cstring>
#include <cassert>

#include <sys/socket.h>

#include <iostream>
#include <string>

#include <jsoncpp/json/json.h> // 表示层：json,protbuf

enum
{
    OK = 0,
    DIVZERO, // 除0️错误
    MODZERO, // 模零错误
    OPERROR  // 操作符错误
};

#define SEP " "
#define SEP_LEN strlen(SEP)

#define LINE_SEP "\r\n"
#define LINE_SEP_LEN strlen(LINE_SEP)

/*
 ****************************************************************
 * **************************************************************
 * 协议定制*
 * **************************************************************
 * **************************************************************
 */

// 收包
//  "textlen/r/nx op y/r/n" or "textlen/r/n_exit_code _result/r/n"
bool ReceivePackage(int sock, std::string &inbuffer, std::string *req_text)
{

    // "textlen/r/nx op y/r/n""textlen/r/nx op y/r/n""textlen/r/nx op y/r/n""textlen/r/nx op y/r/n""textlen/r/nx op y/r/n"
    char buffer[1024];
    while (true)
    {
        ssize_t n = recv(sock, buffer, (sizeof buffer) - 1, 0);
        if (n <= 0)
        {
            // 无数据
            return false;
        }

        buffer[n] = 0;
        inbuffer += buffer;

        auto sep_pos = inbuffer.find(LINE_SEP);
        if (sep_pos == std::string::npos)
        {
            continue;
        }

        // 一定可以知道该报文的长度
        std::string textlen = inbuffer.substr(0, sep_pos);
        int a_text_size = textlen.size() + 2 * LINE_SEP_LEN + stoi(textlen);

        if (inbuffer.size() < a_text_size)
        {
            continue;
        }
        else
        {
            std::cout << "处理前#inbuffer:\n"
                      << inbuffer << std::endl;
            // 至少可以读取到一个完整的报文
            std::string test = inbuffer.substr(0, a_text_size);
            *req_text = test;
            inbuffer.erase(0, a_text_size);
            std::cout << "处理后#inbuffer:\n"
                      << inbuffer << std::endl;

            return true;
        }
    }
}

// 加密
// "x op y" -> "textlen/r/nx op y/r/n"
// "_exit_code _result" -> "textlen/r/n_exit_code _result/r/n"
const std::string EncryptText(const std::string &text)
{
    // std::cout << text << std::endl;

    std::string encrypt_text = std::to_string(text.size());
    // std::cout << encrypt_text ;

    encrypt_text += LINE_SEP;
    // std::cout << encrypt_text  ;

    encrypt_text += text;
    // std::cout << encrypt_text;

    encrypt_text += LINE_SEP;
    // std::cout << encrypt_text ;

    return encrypt_text;
}

// 解密
// "textlen/r/nx op y/r/n" -> "x op y"
// "textlen/r/n_exit_code _result/r/n" -> "_exit_code _result"
const std::string DecryptText(const std::string &package)
{
    // std::cout << package << std::endl;
    auto sep_line_pose = package.find(LINE_SEP);
    assert(sep_line_pose != std::string::npos);
    std::string textlen = package.substr(0, sep_line_pose);
    std::string decrypt_text = package.substr(sep_line_pose + LINE_SEP_LEN, stoi(textlen));
    return decrypt_text;
}

class Request
{
public:
    Request()
        : _x(0), _op(0), _y(0)
    {
    }

    Request(int x, char op, int y)
        : _x(x), _op(op), _y(y)
    {
    }

    // 序列化
    bool serialize(std::string *out)
    {
#ifdef MYSELF

        // 1、自己写
        std::string serialized_str;
        // Request 结构化 -》"x op y"
        std::string x_string = std::to_string(_x);
        std::string y_string = std::to_string(_y);

        serialized_str = x_string;
        serialized_str += SEP;
        serialized_str += _op;
        serialized_str += SEP;
        serialized_str += y_string;
        *out = serialized_str;

#else
        // 2、用现成的
        Json::Value root;
        root["x"] = _x;
        root["op"] = _op;
        root["y"] = _y;

        Json::FastWriter writer;
        *out = writer.write(root);

#endif
        return out;
    }

    // 反序列化
    bool deserialize(const std::string &in)
    {
#ifdef MYSELF

        // "x op y" -》结构化
        auto left = in.find(SEP);
        auto right = in.rfind(SEP);

        if (left == std::string::npos || std::string::npos == right)
        {
            return false;
        }

        if (left == right)
        {
            return false;
        }
        std::string x_string = in.substr(0, left); // [0,2) [start,end) ，start，end - start
        _op = in[left + SEP_LEN];
        std::string y_string = in.substr(right + SEP_LEN);

        if (x_string.empty() || y_string.empty())
        {
            return false;
        }

        _x = stoi(x_string);
        _y = stoi(y_string);
#else
        Json::Value root;
        Json::Reader reader;
        reader.parse(in, root);

        _x = root["x"].asInt();
        _op = root["op"].asInt();
        _y = root["y"].asInt();

#endif
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

    Response(int exitcode, int result)
        : _exit_code(exitcode), _result(result)
    {
    }

    // 序列化
    bool serialize(std::string *out)
    {
#ifdef MYSELF

        // Response -> "_exit_code _result"

        std::string ec_string = std::to_string(_exit_code);
        std::string res_string = std::to_string(_result);
        *out = ec_string;
        *out += SEP;
        *out += res_string;
#else
        // 2、用现成的
        Json::Value root;
        root["exit_code"] = _exit_code;
        root["result"] = _result;

        Json::FastWriter writer;
        *out = writer.write(root);
#endif
        return true;
    }

    // 反序列化
    bool deserialize(const std::string &in)
    {
#ifdef MYSELF

        // "_exit_code _result" -> Response

        auto mid = in.find(SEP);
        if (mid == std::string::npos)
        {
            return false;
        }
        std::string exit_code = in.substr(0, mid);
        std::string result = in.substr(mid + SEP_LEN);
        if (exit_code.empty() || result.empty())
        {
            return false;
        }
        _exit_code = stoi(exit_code);
        _result = stoi(result);
#else
        Json::Value root;
        Json::Reader reader;
        reader.parse(in, root);

        _exit_code = root["exit_code"].asInt();
        _result = root["result"].asInt();

#endif
        return true;
    }

    int _exit_code; // 0 计算成功，！0表示计算失败
    int _result;    // 最终的计算结果
};
