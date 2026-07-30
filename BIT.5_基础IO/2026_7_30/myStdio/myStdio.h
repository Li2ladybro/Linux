#pragma once

#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include <stdlib.h>

// 刷新策略
#define SIZE      1024
#define SYNC_NOW  1<<0
#define SYNC_LINE 1<<1
#define SYNC_FULL 1<<2

typedef struct _FILE
{
    int flags;               // 刷新策略
    int fileno;              // 文件描述符(fd)
    int cap;                 // buffer总量
    int size;                // 用了多少
    char buffer[SIZE];       // 缓冲区
}FILE_;

FILE_ *fopen_(const char *pathname, const char *mode);
void fflush_(FILE_ *fp);
void fwrite_(const void *ptr,int num,FILE_ *fp);
void fclose_(FILE_ *fp);
