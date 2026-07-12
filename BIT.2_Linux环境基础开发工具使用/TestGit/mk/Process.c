#include "Process.h"
#include <stdio.h>

void MyProcess()
{
    int cnt =0;
    char Bar[NUM];
    const char* Lable="|/-\\";
    memset(Bar,'\0', sizeof(Bar));
    while( cnt<NUM )
    {
        printf("\033[47;31m[%-100s][%d%%]%c\r\033[0m",Bar,cnt,Lable[cnt%4]);

        Bar[cnt++]='#';
        fflush(stdout);
        usleep(50000);
    }
    printf("\n");
}
