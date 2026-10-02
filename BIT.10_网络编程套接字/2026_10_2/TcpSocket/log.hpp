#pragma once

#include <iostream>
#include <string>

using namespace std;

#define DEBUG 0
#define NORMAL 1
#define WARNING 2
#define ERR0R 3
#define FATAL 4


void logMessage(int leve,const string& message)
{
    // [日志等级] [时间戳/时间] [pid] [message]
    // [WARNING] [2026-10-2 20:05:56] [1234] [创建socket失败]
    cout<<message<<endl;
}