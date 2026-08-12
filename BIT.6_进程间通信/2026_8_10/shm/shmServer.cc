#include "comm.hpp"

int main()
{
    key_t k = getKey();
    printf("k:0x%x\n", k);
    // 创建共享内存
    int shmid = creatShm(k);
    printf("shmid:%d\n", shmid);
    // sleep(5);
    // 挂接共享内存
    char *start = (char *)shmAttach(shmid);
    printf("shmAttach success ,adderss start:%p\n", start);
    // 进行通信
    while (true)
    {

        printf("client say# %s\n",start);
        struct shmid_ds ms;
        shmctl(shmid,IPC_STAT,&ms);
        printf("共享内存属性：size:%ld,cpid:%d,mypid:%d,key:%x,mykey:%x\n",
            ms.shm_segsz,ms.shm_cpid,getpid(),ms.shm_perm.__key,k);
        sleep(1);
    }
    
    // 去关联
    shmDetach(start);
    sleep(5);
    delShm(shmid);
    return 0;
}