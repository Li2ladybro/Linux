#include "comm.hpp"

// 服务器端
int main()
{

    bool p = createFifo(NAMEDPIPE);
    assert(p == true);

    cout << "server begin\n";
    int rfd = open(NAMEDPIPE, O_RDONLY);
    cout << "server end\n";

    if (rfd == -1)
    {
        cout << "只读打卡文件失败\n";
        exit(-1);
    }

    assert(rfd != -1);

    // read
    char buffer[1024];

    while (true)
    {

        ssize_t n = read(rfd, buffer, sizeof(buffer));
        if (n)
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

    removeFifo(NAMEDPIPE);

    return 0;
}
