
#include "comm.hpp"

// 客户端
int main()
{
    bool p = createFifo(NAMEDPIPE);
    assert(p == true);

    cout<<"client begin\n";

    int wfd = open(NAMEDPIPE, O_WRONLY);
    cout<<"client end\n";

    assert(wfd != -1);

    // write
    while (true)
    {
        /* code */
        cout<<"please say#：";
        char buffer[1024];
        fgets(buffer, sizeof buffer, stdin);
        ssize_t n = write(wfd, buffer, sizeof buffer);
    }

    close(wfd);

    removeFifo(NAMEDPIPE);
    return 0;
}