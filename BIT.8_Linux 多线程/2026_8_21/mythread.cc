#include <iostream>

#include <pthread.h>
#include <unistd.h>

using namespace std;

int g_l = 0;

void *start_routine(void *argv)
{
    int c = 0;

    while (c <= 10)
    {

        cout /*<< (const char *)argv */ << " g_l : " << g_l++ << " start_routine\n";
        c++;
        // sleep(1);
    }
}

int main()
{
    pthread_t tid;
    int n = pthread_create(&tid, nullptr, start_routine, (void *)"吃了吗");
    if (n != 0)
    {
        exit(-1);
    }
    int c = 0;
    while (c <= 10)
    {

        char tidBuffer[64];
        int n = snprintf(tidBuffer, sizeof tidBuffer, "0x%x", tid);
        cout << /*"tid " << tidBuffer << " n " << n << */ " g_l : " << g_l++ << " main\n";
        c++;
        // sleep(1);
    }

    // cout << "hifh\n";
    return 0;
}
