#include <stdio.h>
#include <unistd.h>

int main()
{
    while(1)
    {
        printf("I am a process,pid is %d,ppid is %d\n",getpid(),getppid());
        sleep(1);
    }
    return 0;
}
