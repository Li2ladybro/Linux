#include "TcpClient.hpp"
#include <memory>

using namespace Client;

static void Usage(string proc)
{
    // fprintf(stderr, "\nUsage:\n\t%s server_ip server_port\n\n", proc.c_str());
    // fflush(stderr);
    cout << "\nUsage:\n\t" << proc << " server_ip server_port\n\n";
}

// ./TcpClint serverip serverport
int main(int argc, char *argv[])
{

    if (argc != 3)
    {
        Usage(argv[0]);
        exit(UASGE_ERR);
    }

    string serverip = argv[1];
    uint16_t serverport = atoi(argv[2]);
    unique_ptr<TcpClient> tcli(new TcpClient(serverip, serverport));
    tcli->initClient();
    tcli->start();
    return 0;
}
