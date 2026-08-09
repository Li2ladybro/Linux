#pragma once

#include <cerrno>
#include <cstring>
#include <cassert>
#include <iostream>
#include <string>
#include <cstdlib>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#define NAMEDPIPE "/tmp/namedpipe"

using namespace std;

bool createFifo(const char *path)
{
    umask(0);
    int n = mkfifo(path, 0600);
    if (n == 0)
    {
        return true;
    }
    else
    {
        cout << "errno：" << errno << "err string：" << strerror(errno) << endl;
        return false;
    }
}

void removeFifo(const char *path)
{
    int n = unlink(path);
    assert(n == 0);
}