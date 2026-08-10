#pragma once

#include <cerrno>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <iostream>

#include <sys/shm.h>

using namespace std;

#define PATHNAME "."
#define PROJ_ID 0x66
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

int shmHelper(key_t k, int flag)
{
    // k需要被shmget，设置进入到共享内存的属性中，属于描述该内存的一部分
    //             shmId vs key
    //                fd vs inode
    // (进程管理级别的标识)vs(文件管理级别的标识)
    //             用户层 vs 内核层
    //               钥匙 vs 锁 
    int shmId = shmget(k, MAXSIZE, flag);
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
    return shmHelper(k, IPC_CREAT | IPC_EXCL);
}

// 获取共享内存
int getShm(key_t k)
{
    return shmHelper(k, IPC_CREAT);
}