#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

#define NUM 10
    
typedef void (*func_t)(); // func_t = 函数指针

func_t addressTask[NUM];

// 样例任务
void task1();
void task2();
void task3();
void task4();
void task5();

// 载入任务
void loadTask();

int main()
{
    pid_t id = fork();
    if(id == 0)
    {
        // 子进程
        int cun=10;
        while (cun)
        {
            printf("child running....,pid:%d,ppid:%d,cun:%d\n",getpid(),getppid(),cun--);
            sleep(3);
        }
        exit(22);
    }
    // 父进程
    loadTask();
    int status;
    while(1)
    {
         pid_t ret = waitpid(id,&status,WNOHANG);
         if(ret == 0)
         {
             printf("wait done,but child is running....\n");
             // 如果子进程在忙，父进程也可以做自己的事
             for(int i = 0;addressTask[i];++i)
                 // 采用回调方式，执行父进程的任务
                 addressTask[i]();
         }
         if(ret > 0)
         {
            printf("wait success,exit code:%d,sig code:%d\n",(status>>8)&0xff,status&0x7f);
            break;
         }
         if(ret<0)
         {
            printf("wait error\n");
           exit(-1); 
         }
         sleep(1);

    }
    return 0;
}

// 样例任务
void task1()
{
    printf("address task1\n");
}

void task2()
{
    printf("address task2\n");
}

void task3()
{
    printf("address task3\n");
}

void task4()
{
    printf("address task4\n");
}

void task5()
{
    printf("address task5\n");
}

// 载入任务
void loadTask()
{
    memset(addressTask,0,sizeof(addressTask));
    addressTask[0]=task1;
    addressTask[1]=task2;
    addressTask[2]=task3;
    addressTask[3]=task4;
    addressTask[4]=task5;
}

