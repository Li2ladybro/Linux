#include "TcpServer.hpp"
#include <memory>

using namespace Server;

static void Usage(string proc)
{
    cout << "\nUsage:\n\t" << proc << " Iocal_port\n\n";
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

    std::unique_ptr<TcpServer> tsvr(new TcpServer(port));
    tsvr->initServer();
    tsvr->start();
    return 0;
}
