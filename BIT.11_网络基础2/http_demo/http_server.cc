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
bool LogicHander(HttpRequest &req, HttpResponse &resp)
{
    cout << "\n\n\n\n\n\n\n----------------start----------------\n";

    cout << req.inbufer;

    req.parse();

    cout << "<req.method：" << req.method << endl;
    cout << "<req.url：" << req.url << endl;
    cout << "<req.httpversion：" << req.httpversion << endl;

    cout << "-----------------end-----------------\n";

    string respline = "HTTP/1.1 200 OK\r\n";
    string respheader = "Content-Type:text/html\r\n";
    string resblank = "\r\n";
    string boby = "Hello Word";

    resp.outbuffer = respline;
    resp.outbuffer += respheader;
    resp.outbuffer += resblank;
    resp.outbuffer += boby;

    return true;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        Usage(argv[0]);
        exit(-1);
    }

    unique_ptr<HttpServer> user(new HttpServer(atoi(argv[1]), LogicHander));
    user->initServer();
    user->start();

    return 0;
}