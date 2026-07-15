#include <stdio.h>
#include <unistd.h>

int main()
{
    int p=fork();
    if(p>0)
    {
        while(1)
        {
            printf("我是父进程，pid：%d，ppid：%d\n",getpid(),getppid());
            sleep(1);
        }
    }
    else if(p==0)
    {
        while(1)
        { 
            printf("我是子进程，pid：%d，ppid：%d\n",getpid(),getppid());
            sleep(3);
        }
    }
    else
    {
        return -1;
    }

    return 0;
}
