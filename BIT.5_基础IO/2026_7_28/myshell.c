#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <pwd.h>
#include <assert.h>

#define OPT_MAX 64
#define LINECHARMAX 1024

// 重定向类型
#define NONE_REDIR   0     // 没有重定向
#define INPUT_REDIR  1     // 输入重定向     "<"    cat < file.txt
#define OUTPUT_REDIR 2     // 输出重定向     ">"    "ls -a" > file.txt
#define APPEND_REDIR 3     // 追加重定向     ">>"   "ls -a" >> file.txt

#define trimSpace(start)   do{\
            while(isspace(*start)) ++start;\
        }while(0)  // 切割空格


void commandCheck(char *commands);  // 文件重定向
int redirType;                      // 需要怎么重定向
char *redirFile;                    // 重定向的文件

char lineCommand[LINECHARMAX];      // 接收的输入的命令
char *myargv[OPT_MAX];              // 分割用户命令

char MYUSER[1024];                  // 用户名
char MYHOSTNAME[1024];              // 主机名
char MYPWD[1024];                   // PWD  

int lastExitCode;                   // 退出码
int lastSigCode;                    // 信号码


int main()
{
    
    while(1)
    {
        redirType = NONE_REDIR;         // 需要怎么重定向
        redirFile = NULL;             // 重定向的文件

        gethostname(MYHOSTNAME,sizeof(MYHOSTNAME));
        getcwd(MYPWD,sizeof(MYPWD));

        uid_t uid = getuid();
        struct passwd *pw = getpwuid(uid);
        strcpy(MYUSER,pw->pw_name);

        // 输出命令提示符
        /*printf("%s@%s:%s$ ",getenv("USER"),MYHOSTNAME,MYPWD);*/
        printf("%s@%s:%s$ ",MYUSER,MYHOSTNAME,MYPWD);

        // 获取用户输入
        char *s = fgets(lineCommand,sizeof(lineCommand)-1,stdin);
        assert(s != NULL);
        // 清除最后一个\n，补充成'\0'，abc\n
        lineCommand[strlen(lineCommand)-1] = '\0';

        commandCheck(lineCommand);

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
            // 命令是子进程执行的，所以真正重定向的工作一定要由子进程完成
            // 如何重定向呢？需要父进程bash提供信息
            // 这里重定向会影响父进程吗？-》不会
            // 子进程会拷贝文件描述符表但是不会拷贝打开的文件
            switch (redirType)
            {
                case NONE_REDIR:
                    break;

                // "cat < file.txt" 输入重定向
                case INPUT_REDIR:
                    {
                         int fd = open(redirFile,O_RDONLY);
                         if(fd == -1)
                         {
                             perror("open failed");
                             exit(errno);
                         }

                         dup2(fd ,0);
                    }
                    break;

                // "ls -a -l -d > file.txt"  输出重定向
                // "ls -a -l -d >> file.txt" 追加重定向
                case OUTPUT_REDIR:
                case APPEND_REDIR:
                    {
                        int flags = O_WRONLY | O_CREAT;
                        if(redirType == APPEND_REDIR)
                        {
                            flags |= O_APPEND;
                        }
                        else 
                        {
                            // 重写
                            flags |= O_TRUNC;
                        }

                        int fd = open(redirFile,flags,0666);
                        if(fd == -1)
                        {
                            perror("open failed");
                            exit(errno);
                        }

                        dup2(fd,1);
                    }
                    break;

                default:
                    printf("errno\n");
                    break;
            }
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

// "ls -a -l -d > file.txt"  -> 命令 + 文件: "ls -a -l -d"  "file.txt"
void commandCheck(char *commands)
{
    assert(commands);
    char *start = commands;
    char *end = commands + strlen(commands);
    while(start != end)
    {
        // "ls -a -l -d > file.txt"  输出重定向
        // "ls -a -l -d >> file.txt" 追加重定向
        if(*start == '>')
        {
            *start = '\0';
            start++;
            if(*start == '>')
            {
                redirType = APPEND_REDIR;
                start++;
            }
            else
            {
                redirType = OUTPUT_REDIR;
            }
            trimSpace(start);
            redirFile = start;
            break;

        }
        // "cat < file.txt" 输入重定向
        else if(*start == '<')
        {
            *start = '\0';
            start++;
            trimSpace(start);
            // 填写重定向信息
            redirType = INPUT_REDIR;
            redirFile = start;
            break;
        }
        else
        {
            start++;
        }
    }
}
