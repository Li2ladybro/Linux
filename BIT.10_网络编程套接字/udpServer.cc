#include "udpServer.hpp"
#include <memory>

using namespace Server;


static void Usage(string proc)
{
    cout << "\nUsage:\n\t" << proc << " Iocal_port\n\n";
}

// ./udpServer ip port
int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        Usage(argv[0]);
        exit(-1);
    }
    uint16_t port = atoi(argv[1]);
    // string ip = argv[1];

    std::unique_ptr<udpServer> usvr(new udpServer(port));
    usvr->initServer();
    usvr->start();
    return 0;
}
