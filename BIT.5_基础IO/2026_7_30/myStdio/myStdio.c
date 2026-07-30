#include "myStdio.h"
#include <string.h>
#include <unistd.h>
#include <wchar.h>

FILE_ *fopen_(const char *pathname, const char *mode)
{
    int flags = 0;
    int defautMode = 0666;
    if (strcmp(mode, "r") == 0)
    {
        flags |= O_RDONLY;
    }
    else if (strcmp(mode, "w") == 0) 
    {
        flags |= (O_WRONLY | O_CREAT);
    }
    else if (strcmp(mode , "a") == 0)
    {
        flags |= (O_WRONLY | O_CREAT | O_TRUNC);
    }

    int fd = 0;
    if(flags & O_RDONLY)
    {
        fd = open(pathname,flags);
    }
    else 
    {
        fd = open(pathname,flags,defautMode);
    }
    
    if(fd == -1)
    {
        char *erro = strerror(errno);
        write(2, erro, strlen(erro));
        return NULL;
    }
    FILE_ *fp =(FILE_*)malloc(sizeof(FILE_));
    assert(fp);
    fp->flags = SYNC_LINE;
    fp->fileno = fd;
    fp->cap = SIZE;
    fp->size = 0;
    memset(fp->buffer, 0, SIZE);
    return fp;
}

void fflush_(FILE_ *fp)
{
    if(fp->size)
    {
         // 1 将数据写入内核缓冲区
         write(fp->fileno,fp->buffer,fp->size); 
         fp->size = 0;
         // 2 强制刷新，将内核缓冲区数据强制刷新到磁盘
         fsync(fp->fileno); 
    }
}

void fwrite_(const void *ptr,int num,FILE_ *fp)
{
    // 写入缓冲区
    memcpy(fp->buffer+fp->size,ptr,num);
    fp->size += num;

    if(fp->flags & SYNC_FULL)
    {
        if(fp->size == fp->cap)
        {
            write(fp->fileno,fp->buffer,fp->size);
            fp->size = 0;
        }
    }
    else if(fp->flags & SYNC_LINE)
    {
        if(fp->buffer[fp->size-1] == '\n')
        {
            write(fp->fileno,fp->buffer,fp->size);
            fp->size = 0;
        }
    }
    else if (fp->flags & SYNC_NOW)
    {
    
        write(fp->fileno,fp->buffer,fp->size);
        fp->size = 0;
    }
    else
    {
        // NODO 
    }
}
void fclose_(FILE_ *fp)
{
    fflush_(fp);
    close(fp->fileno);
}
