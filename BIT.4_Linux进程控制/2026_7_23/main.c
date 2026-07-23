#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main()
{
    printf("process is running....\n");
    pid_t id = fork();

    if(id == 0)
    {
        extern char** environ;
        // 子进程
        sleep(1);
        char* const argv[] =
        {
            "pwd",
            NULL
        };
        putenv("MYENV=2026_7_23");
        /*execl("./mybin.exe","mybin.exe",NULL);*/
        execle("./mybin.exe","mybin.exe",NULL,environ);
        /*execlp("ls","ls","-al",NULL);*/
        /*execv("/usr/bin/pwd",argv);*/
        /*execvp("pwd",argv);*/
        perror("execle");
        exit(1);
    }
    int stutas;
    pid_t ret = waitpid(id,&stutas,0); 
    
     if(ret == id)
    {
        printf("wait success,exit code:%d,sig code:%d\n",(stutas>>8)&0xff,stutas&0x7f);
    }
    return 0;
}
