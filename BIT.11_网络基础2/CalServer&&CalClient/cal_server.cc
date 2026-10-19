#include "cal_server.hpp"
#include "protocol.hpp"

#include <memory>

using namespace Server;

static void Usage(string proc)
{
    cout << "\nUsage:\n\t" << proc << " Iocal_port\n\n";
}

// req：已经是处理好的一个完整请求对象
// resp：根据req，进行业务处理，填充resp，不需要管任何读取和写入，序列化、反序列化等细节
bool cal(const Request &req, Response &resp)
{
    // 应用层，业务逻辑处理
    resp._exit_code = OK;
    resp._result = OK;
    switch (req._op)
    {

    case '+':
        resp._result = req._x + req._y;
        break;

    case '-':
        resp._result = req._x - req._y;
        break;

    case '*':
        resp._result = req._x * req._y;
        break;

    case '/':
        if (req._y == 0)
        {
            resp._exit_code = DIVZERO;
        }
        else
        {
            resp._result = req._x / req._y;
        }
        break;

    case '%':
        if (req._y == 0)
        {
            resp._exit_code = MODZERO;
        }
        else
        {
            resp._result = req._x / req._y;
        }
        break;

    default:
        resp._exit_code = OPERROR;
    }
    return true;
}

// ./TcpServer local_port
int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        Usage(argv[0]);
        exit(UASGE_ERR);
    }
    uint16_t port = atoi(argv[1]);

    std::unique_ptr<CalServer> tsvr(new CalServer(port));
    tsvr->initServer();
    // DaemonSelf();
    tsvr->start(cal);
    return 0;
}
