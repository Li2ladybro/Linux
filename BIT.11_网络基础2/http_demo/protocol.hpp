#pragma once

#include "util.hpp"

#include <iostream>
#include <string>
#include <sstream>

const std::string KSep_Line = "\r\n";
const std::string KDefault_Path = "./wwwwork";
const std::string KHome_Page = "index.html";
const std::string K404_Page = "./wwwwork/404.html";

class HttpRequest
{

public:
    // std::string reqqline;
    // std::vector<std::string> reheader;
    // std::string body;
    void parse()
    {
        // XXXX XXX XXX \r\n

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
        // ./wwwwork/
        if (path[path.size() - 1] == '/')
        {
            path += KHome_Page;
        }

        // 4、后缀
        auto pos = path.rfind('.');
        if (pos == std::string::npos)
        {       
            suffix = "";
        }
        else
        {
            suffix = path.substr(pos);
        }
    }

    std::string method;// 请求方法
    std::string url;// 请求地址
    std::string httpversion;// 版本号

    std::string path;// 完整的工作路径
    std::string suffix;//   文件后缀

    std::string inbufer; // 存储请求
};

class HttpResponse
{
public:
    std::string outbuffer; // 存储响应
};
