#pragma once

#include <ctime>
#include <cassert>
#include <cstring>
#include <cstdarg> // 提供 va_start, va_list, va_end

#include <iostream>
#include <string>

#include <unistd.h>
#include <fcntl.h>

using namespace std;

#define DEBUG 0
#define NORMAL 1
#define WARNING 2
#define ERR0R 3
#define FATAL 4

#define LOG_NORMAL "log.normal"
#define LOG_ERROR "log.error"

// logMessage(NORMAL, "Create Socket Success _listenSockfd: %d",_listenSockfd);

const char *LeveToString(int leve)
{
    switch (leve)
    {
    case 0:
        return "DEBUG";
    case 1:
        return "NORMAL";
    case 2:
        return "WARNING";
    case 3:
        return "ERR0R";
    case 4:
        return "FATAL";
    default:
        return nullptr;
    }
}

void logMessage(int leve, const char *format, ...)
{
    // format="Create Socket Success _listenSockfd: %d"
    // ...=_listenSockfd
    // [日志等级] [时间戳/时间] [pid] [message]
    // [WARNING] [2026-10-2 20:05:56] [1234] [创建socket失败]

#define NUM 1024
    char logprefix[NUM];
    time_t tim = (time(nullptr));
    struct tm *now = localtime(&tim);

    snprintf(logprefix, sizeof logprefix, "[%s][%04d-%02d-%02d %02d:%02d:%02d][%d]",
             LeveToString(leve),
             now->tm_year + 1900,
             now->tm_mon + 1,
             now->tm_mday,
             now->tm_hour,
             now->tm_min,
             now->tm_sec,
             getpid());

    char logcontent[NUM];
    va_list arg;
    va_start(arg, format); // 使得 arg 指向_listenSockfd
    vsnprintf(logcontent, sizeof logcontent, format, arg);

    FILE *log_normal = fopen(LOG_NORMAL, "a");
    FILE *log_error = fopen(LOG_ERROR, "a");
    assert(log_error && log_normal);
    FILE *cur = nullptr;
    if (leve == DEBUG || leve == NORMAL || leve == NORMAL)
    {
        cur = log_normal;
    }
    if (leve == ERR0R || leve == FATAL)
    {
        cur = log_error;
    }
    if (cur)
    {
        fprintf(cur, "%s%s\n", logprefix, logcontent);
    }
    fclose(log_normal);
    fclose(log_error);
}