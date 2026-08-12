#include "comm.hpp"

int main()
{
    key_t k = getKey();
    printf("k:0x%x\n", k);

    int shmid = getShm(k);
    printf("shmid:%d\n", shmid);

    // 挂接共享内存
    char *start = (char *)shmAttach(shmid);
    printf("shmAttach success ,adderss start:%p\n", start);

    const char *messages = "hello server,我是另一个进程，正在与你通信";
    pid_t id = getpid();
    int cnt = 0;
    // char buffer[1024];
    // 进行通信
    while (true)
    {
        snprintf(start, MAXSIZE, "%s[pid:%d][消息编号:%d]", messages, id, cnt++);
        sleep(1);
        // snprintf(buffer,sizeof buffer,"%s[pid:%d][消息编号:%d]",messages,id,cnt++);
        // memcpy(start,buffer,strlen(buffer)+1);
    }
    sleep(5);

    // 去关联
    shmDetach(start);
    return 0;
}