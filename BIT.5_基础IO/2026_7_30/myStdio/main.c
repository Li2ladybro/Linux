#include "myStdio.h"
#include <stdio.h>
#include <unistd.h>

int main()
{
    FILE_ *fp = fopen_("./log.txt","w");
    const char *str = "hello 2026_7_30";
    int cnu = 10;
    while(cnu)
    {
        fwrite_(str,strlen(str),fp);
        fflush_(fp);
        printf("cnu:%d\n",cnu--);
        /*if(cnu == 5)*/
        /*{*/
            /*fflush_(fp);*/
        /*}*/
        sleep(1);
    }

    fclose_(fp);
    return 0;
}
