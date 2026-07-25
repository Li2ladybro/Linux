#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define FILE_NAME "log.txt"
int main()
{
    // "w":O_WRONLY | O_CREAT | O_TRUNC+0666
    // "a":O_WRONLY | O_CREAT | O_APPEND
    int fd = open(FILE_NAME,O_WRONLY | O_CREAT | O_APPEND,0666);
    if(fd ==-1)
    {
        perror("open failed");
        exit(-1);
    }
    close(fd);
    return 0;
}

