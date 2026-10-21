#pragma once

#include "util.hpp"

#include <iostream>
#include <string>
#include <sstream>

const std::string KSep_Line = "\r\n";
const std::string KDefault_Path = "./wwwwork";

class HttpRequest
{

public:
    // std::string reqqline;
    // std::vector<std::string> reheader;
    // std::string body;
    void parse()
    { // XXXX XXX XXX \r\n

        // 1、获取第一行
        std::string first_line_str = Util::GetFirstline(inbufer, KSep_Line);
        if (first_line_str.empty() == true)
        {
            return;
        }
        std::cout << first_line_str << std::endl;

        // 2、解析 req 字段
        std::stringstream ss(first_line_str);
        ss >> method >> url >> httpversion;

        // 3、 工作路径
        path = KDefault_Path;
        path += url;
    }

    std::string method;
    std::string url;
    std::string httpversion;

    std::string path;

    
    std::string inbufer; // 存储请求
};

class HttpResponse
{
public:
    std::string outbuffer; // 存储响应
};
