#include "comm.hpp"

int main()
{
    key_t k = getKey();
    printf("k:0x%x\n", k);
    int shmid = creatShm(k);
    printf("shmid:%d\n", shmid);

    return 0;
}