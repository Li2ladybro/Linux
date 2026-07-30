#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main()
{
    // C接口
    printf("hello printf ");
    fprintf(stdout,"hello fprintf ");
    const char *fputsString = "hello fputs ";
    fputs(fputsString,stdout);

    // 系统接口
    const char *wString = "hello write ";
    write(1, wString, strlen(wString));
    
    // 结束前调用子进程
    fork();
    return 0;
}
