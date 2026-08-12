#pragma once

#include <cerrno>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <iostream>

#include <sys/shm.h>s
#include <unistd.h>

using namespace std;

#define PATHNAME "."
#define PROJ_ID 0x66
// 共享内存的大小，一般建议是4KB的整数倍
// 系统分配共享内存是以4KB为单位的! --- 内存划分内存块的基本单位Page
// #define MAX_SIZE 4097         --- 内核给你的会向上取整(8kB)，内核给你的，和你能用的，是两码事

#define MAXSIZE 4096

// 获取同一个唯一key
key_t getKey()
{
    key_t k = ftok(PATHNAME, PROJ_ID);
    if (k == -1)
    {
        cout << "关键字创建失败\n";
        cerr << errno << ":" << strerror(errno) << endl;
        exit(-1);
    }
    else
    {
        return k;
    }
}

int shmHelper(key_t k, int flags)
{
    // k需要被shmget，设置进入到共享内存的属性中，属于描述该内存的一部分
    //             shmId vs key
    //                fd vs inode
    // (进程管理级别的标识)vs(文件管理级别的标识)
    //             用户层 vs 内核层
    //               钥匙 vs 锁
    int shmId = shmget(k, MAXSIZE, flags);
    if (shmId == -1)
    {
        cerr << errno << ":" << strerror(errno) << endl;
        exit(-1);
    }
    else
    {
        return shmId;
    }
}

// 创建一段共享内存
int creatShm(key_t k)
{
    return shmHelper(k, IPC_CREAT | IPC_EXCL | 0600);
}

// 获取共享内存
int getShm(key_t k)
{
    return shmHelper(k, IPC_CREAT);
}

// 挂接共享内存
void *shmAttach(int shmid)
{
    void *mem = shmat(shmid, nullptr, 0);
    if ((long long)mem == -1l)
    {
        cerr << "shmAttach:" << errno << ":" << strerror << endl;
        exit(3);
    }
    return mem;
}

// 去关联
void shmDetach(void *start)
{
    if (shmdt(start) == -1)
    {
        cerr << "shmDetach:" << errno << ":" << strerror << endl;
    }
}

// 销毁共享内存
bool delShm(int shmid)
{
    if (shmctl(shmid, IPC_RMID, nullptr) == -1)
    {
        cerr << errno << ":" << strerror << endl;
        return false;
    }
    return true;
}