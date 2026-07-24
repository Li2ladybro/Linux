#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <assert.h>

#define OPT_MAX 64
#define LINECHARMAX 1024

char lineCommand[LINECHARMAX];      // 接收的输入的命令
char *myargv[OPT_MAX];              // 分割用户命令

char MYHOSTNAME[1024];              // 主机名
char MYPWD[1024];                   // PWD  

int lastExitCode;
int lastSigCode;

int main()
{
    
    while(1)
    {
        gethostname(MYHOSTNAME,sizeof(MYHOSTNAME));
        getcwd(MYPWD,sizeof(MYPWD));

        // 输出命令提示符
        printf("%s@%s:%s$ ",getenv("USER"),MYHOSTNAME,MYPWD);

        // 获取用户输入
        char *s = fgets(lineCommand,sizeof(lineCommand)-1,stdin);
        assert(s != NULL);
        // 清除最后一个\n，补充成'\0'，abc\n
        lineCommand[strlen(lineCommand)-1] = '\0';
        // " ls -l -a -f"
        // 字符串切割
        myargv[0] = strtok(lineCommand," ");
        
        int i = 1;
        if(strcmp(myargv[0], "ls") == 0)
        {
            myargv[i++] = "--color=auto";
        }
        while((myargv[i++] = strtok(NULL," ")));
        // 如果是cd命令，就不需要创建子进程，让Shell自己执行
        // 像这种不需要子进程执行的程序，让Shell自己执行的命令--内置/自建命令
        if(strcmp(myargv[0],"cd") == 0 && myargv[1] != NULL)
        {
            chdir(myargv[1]);
            continue;
        }

        if(strcmp(myargv[0],"echo") == 0)
        {
            if(strcmp(myargv[1],"$?") == 0)
            {
                printf("exit_code:%d,sig_code%d\n",lastExitCode,lastSigCode);
            }
            else
            {
                printf("%s\n",myargv[1]);
            }
            continue;
        }

        // 测试是否切割成功
#ifdef DEBUG
    for(int i = 0 ;myargv[i];++i)
    {
        printf("myargv[%d] = %s\n",i,myargv[i]);
    }
#endif
    
        pid_t id = fork();
        assert(id != -1);

        if(id == 0)
        {
            execvp(myargv[0],myargv);
            exit(1);
        }
        // 父进程
        int status;
        int ret =  waitpid(id,&status, 0);
        assert(ret == id);
        lastExitCode =(status>>8 & 0xff);
        lastSigCode = status & 0x7f;
    }
    return 0;
}
