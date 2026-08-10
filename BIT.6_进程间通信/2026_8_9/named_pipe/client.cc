#include "comm.hpp"

// 客户端
int main()
{

    cout << "client begin\n";
    int wfd = open(NAMEDPIPE, O_WRONLY);
    cout << "client end\n";

    assert(wfd != -1);

    // write
    while (true)
    {
        /* code */
        cout << "please say#：";
        char buffer[1024];
        fgets(buffer, sizeof buffer, stdin);
        int lens = strlen(buffer);
        buffer[lens - 1] = '\0';
        ssize_t n = write(wfd, buffer, lens);
    }

    close(wfd);

    return 0;
}