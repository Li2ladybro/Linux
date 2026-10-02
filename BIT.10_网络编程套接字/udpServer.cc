#include "udpUsers.hpp"
#include "udpServer.hpp"
#include <memory>

using namespace Server;

OnlineUsers gOnlineUser;

static void Usage(string proc)
{
    cout << "\nUsage:\n\t" << proc << " Iocal_port\n\n";
}

void routeMessage(int sockfd, string clientip, uint16_t clientport, string message)
{
    // 对报文业务进行处理，实现server通信与业务逻辑解耦

    if (gOnlineUser.isOnline(clientip, clientport))
    {
        // 群发

        gOnlineUser.broadMessage(sockfd, clientip, clientport, message);
    }

    else if (message == "online")
    {
        gOnlineUser.addUser(clientip, clientport);
    }

    else if (message == "offline")
    {
        gOnlineUser.delUser(clientip, clientport);
    }

    else
    {
        struct sockaddr_in tmp;
        memset(&tmp, 0, sizeof tmp);

        tmp.sin_family = AF_INET;
        tmp.sin_addr.s_addr = inet_addr(clientip.c_str());
        tmp.sin_port = htons(clientport);

        string s = "您还未上线，请先上线（运行online）";
        sendto(sockfd, s.c_str(), s.size(), 0, (struct sockaddr *)&tmp, sizeof tmp);
    }
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

    std::unique_ptr<udpServer> usvr(new udpServer(routeMessage, port));
    usvr->initServer();
    usvr->start();
    return 0;
}
