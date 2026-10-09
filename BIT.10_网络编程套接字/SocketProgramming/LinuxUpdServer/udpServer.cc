#include "udpServer.hpp"
#include <memory>

using namespace Server;

static void Usage(string proc)
{
    cout << "\nUsage:\n\t" << proc << " Iocal_port\n\n";
}

void handleMeseage(int sockfd, string clientip, uint16_t clientport, string message)
{

    struct sockaddr_in tmp;
    memset(&tmp, 0, sizeof tmp);

    tmp.sin_family = AF_INET;
    tmp.sin_addr.s_addr = inet_addr(clientip.c_str());
    tmp.sin_port = htons(clientport);
    string s = "[Server Echo]# " + message;
    sendto(sockfd, s.c_str(), s.size(), 0, (struct sockaddr *)&tmp, sizeof tmp);
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

    std::unique_ptr<udpServer> usvr(new udpServer(handleMeseage, port));
    usvr->initServer();
    usvr->start();
    return 0;
}
