#include "http_server.hpp"

#include <memory>

using namespace std;
using namespace Server;

void Usage(char *proc)
{
    cout << "\nUsage\n\t" << proc << " port \n\r\n\r";
}

// 1、服务器和网页分离，html
// 2、url -> / :web根目录
bool Get(HttpRequest &req, HttpResponse &resp)
{
    cout << "\n----------------reqstart----------------\n";

    cout << req.inbufer;

    req.parse();

    cout << "<req.method：" << req.method << endl;
    cout << "<req.url：" << req.url << endl;
    cout << "<req.httpversion：" << req.httpversion << endl;
    cout << "<req.path：" << req.path << endl;
    cout << "<req.suffix：" << req.suffix << endl;
    cout << "<req.size：" << req.size << " Byte" << endl;

    cout << "--------------------reqend----------------\n";

    string respline = "HTTP/1.1 307 Temporary Redirect\r\n";

    string respheader = Util::SuffixToHeaderType(req.suffix);

    string content_len = "Content-Length: " + to_string(req.size) + "\r\n";

    string resblank = "\r\n";
    // string boby = "Hello Word";

    string boby;

    if (false == Util::ReadFile(req.path, &boby))
    {
        Util::ReadFile(K404_Page, &boby);
        respline = "HTTP/1.1 404 Not Found\r\n";
        content_len = "Content-Length: " + to_string(boby.size()) + "\r\n";
    }

    resp.outbuffer = respline;
    resp.outbuffer += respheader;
    resp.outbuffer += content_len;

    resp.outbuffer += "Location: http://www.baidu.com\r\n"; // 响应添加临时重定向

    resp.outbuffer += resblank;
    resp.outbuffer += boby;

    cout << "\n--------------respstart----------------\n";
    cout << respheader;
    cout << "\n---------------respend-----------------\n";

    // std::cout<<"resp.outbuffer:\n"<<resp.outbuffer<<std::endl;

    return true;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        Usage(argv[0]);
        exit(-1);
    }

    unique_ptr<HttpServer> user(new HttpServer(atoi(argv[1]), Get));
    // unique_ptr<HttpServer> user(new HttpServer(atoi(argv[1])));

    // 功能路由
    // user->RegisterCallBack("/wwwwork", Get);

    user->initServer();
    user->start();

    return 0;
}