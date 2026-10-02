#pragma warning (disable:4996)

#include <iostream>
#include <WinSock2.h>
#include <string>
#include <cstdio>

#pragma comment(lib, "ws2_32.lib")
using namespace std;

uint16_t serverPort = 8080;
string serverip = "10.48.165.157";

int main()
{
    WSAData wsd;
    if (WSAStartup(MAKEWORD(2, 2), &wsd) != 0)
    {
        cout << "WSAStartup Error = " << WSAGetLastError() << endl;
        return 0;
    }
    else
    {
        cout << "WSAStartup Success\n";
    }

    SOCKET csock = socket(AF_INET, SOCK_DGRAM, 0);
    if (csock == -1)
    {
        cout << "Sock Error\n";
        return 1;
    }
    else
    {
        cout << "Sock Success\n";
    }
    struct sockaddr_in server;
    int len = sizeof server;

    memset(&server, 0, len);
    server.sin_port = htons(serverPort);
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(serverip.c_str());

    string line;
    char buffer[1024];
    while (true)
    {
        cout << "Please Enter# ";
        getline(cin, line);
        int n = sendto(csock, line.c_str(), sizeof line, 0, (struct sockaddr*)&server, len);
        if (n < 0)
        {
            cerr << "Send Error!\n";
            break;
        }
        // 接收数据

        struct sockaddr_in peer;
        int plen = sizeof peer;
        buffer[0] = 0;
        int s = recvfrom(csock, buffer, sizeof buffer - 1, 0, (sockaddr*)&peer, &plen);
        if (s > 0)
        {

            buffer[s] = 0;
            cout << "server 返回的消息是# " << buffer << endl;
    }
        else
        {
            break;
        }
    }
    closesocket(csock);
    return 0;
}
