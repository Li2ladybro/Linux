#include <cassert>
#include <cstring>

#include <iostream>
#include <string>

#include <pthread.h>
#include <unistd.h>

using namespace std;

string changeTid(const pthread_t &thread_id)
{
    char tid[128];
    snprintf(tid, 128, "0x%x", thread_id);
    return tid;
}

void *start_routine(void *argv)
{
    // 自己释放资源，主线进程不关心退出值
    // pthread_detach(pthread_self()) ; // 自己分离自己
    int num = 4;
    while (num--)
    {
        cout << changeTid(pthread_self()) << (char *)argv << " running...\n";
        sleep(1);
    }
    return nullptr;
}

int main()
{
    pthread_t tid;
    int n = pthread_create(&tid, nullptr, start_routine, (void *)"thread one");
    assert(n == 0);
    pthread_detach(tid);

    cout << changeTid(pthread_self()) << " main running...new thread id : " << changeTid(tid) << endl;
    // 一个线程默认是joinable的，如果设置了分离状态，不能够进行等待了!
    int res = pthread_join(tid, nullptr);
    cout << "pthread_join " << res << " : " << strerror(res) << endl;

    return 0;
}