#include "comm.hpp"

// 服务器端
int main()
{

    cout<<"server begin\n";
    int rfd = open(NAMEDPIPE, O_RDONLY);
    if(rfd==-1)
    {
        cout<<"只读打卡文件失败\n";
        exit(-1);
    }
    cout<<"server end\n";

    assert(rfd != -1);

    // read
    while (true)
    {
        char buffer[1024];
        ssize_t n = read(rfd, buffer, strlen(buffer));
        if (n == strlen(buffer))
        {
            cout << "server->#：" << buffer << endl;
        }
        else
        {
            cout << "client quit me too\n";
            break;
        }
    }

    close(rfd);

    return 0;
}
