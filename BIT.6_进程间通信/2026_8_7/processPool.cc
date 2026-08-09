#include <cstdlib>
#include <cassert>
#include <cstdio>
#include <ctime>
#include <iostream>
#include <vector>
#include <string>
#include <unistd.h>
#include <sys/wait.h>

using namespace std;

#define MAKESEED() srand((unsigned)time(nullptr) ^ getpid() ^ 0x13333 ^ rand() % 1234)
#define PROCESS_NUM 5

/* 模拟子进程完成某种任务*/
// 函数指针
typedef void (*func_t)();

void downLoadTask()
{
    cout << getpid() << " ：do void downLoadTask()\n\n";
    sleep(1);
}

void ioTask()
{
    cout << getpid() << " : do void ioTask()\n\n";
    sleep(1);
}

void flushTask()
{
    cout << getpid() << " : do void flushTask()\n\n";
    sleep(1);
}

void loadTask(vector<func_t> *funcMap)
{
    assert(funcMap);
    funcMap->push_back(downLoadTask);
    funcMap->push_back(ioTask);
    funcMap->push_back(flushTask);
}

/* 下面的代码是一个多进程程序 */
class subEp // EndPoint
{
public:
    subEp(pid_t subId, int writeFd)
        : _subId(subId), _writeFd(writeFd)
    {
        char nameBuffer[1024];
        snprintf(nameBuffer, sizeof nameBuffer, "process %d[pid(%d)-fd(%d)]", _num++, _subId, _writeFd);
        _name = nameBuffer;
    }

    const string getsubEpName() const
    {
        return _name;
    }

    const pid_t getsubEpId() const
    {
        return _subId;
    }

    const int getsunEpWriteFd() const
    {
        return _writeFd;
    }

private:
    static int _num;
    string _name;
    pid_t _subId;
    int _writeFd;
};
int subEp::_num = 0;

int reccTask(const int readfd)
{
    int code = 0;
    ssize_t s = read(readfd, &code, sizeof code);
    if (s == 4)
    {
        return code;
    }
    else if (s <= 0)
    {
        return -1;
    }
    else
    {
        return -1;
    }
}

void sendTask(const subEp &process, int taskIdx)
{
    cout << "send task：" << taskIdx << " send to " << process.getsubEpName() << endl;
    int n = write(process.getsunEpWriteFd(), &taskIdx, sizeof(taskIdx));
    assert(n == sizeof(taskIdx));
}

void creatSubProcess(vector<subEp> *subs, vector<func_t> &funcMap)
{
    vector<int> deleteFd;
    for (int i = 0; i < PROCESS_NUM; ++i)
    {
        int fds[2];
        assert(pipe(fds) == 0);
        pid_t id = fork();
        if (id == 0)
        {
            for(int i=0;i<deleteFd.size();++i)
            {
                close(deleteFd[i]);
            }
            // 子进程读
            close(fds[1]);
            while (true)
            {
                // 1、获取命令码，如果没则阻塞
                int commmandCode = reccTask(fds[0]);

                // 2、完成任务
                if (commmandCode >= 0 && commmandCode < funcMap.size())
                {
                    funcMap[commmandCode]();
                }
                else if (commmandCode == -1)
                {
                    break;
                }
            }
            exit(0);
        }
        // 父进程写
        close(fds[0]);
        subEp sub(id, fds[1]);
        subs->push_back(sub);
        deleteFd.push_back(sub.getsunEpWriteFd());
    }
}

void loadBalanceCtr(const vector<func_t> &funcMap, const vector<subEp> &subs, int taskCnt)
{
    int processNum = subs.size();
    int taskNum = funcMap.size();
    bool forver = taskCnt == 0 ? true : false;
    while (true)
    {
        // 1、选择一个要通信的子进程   ----> vector<subEp>  -> index - 随机数
        int subIdx = rand() % processNum;
        // 2、选择任务                ----> vector<func_t> -> index
        int taskIdx = rand() % taskNum;
        // 3、将任务发送给子进程
        sendTask(subs[subIdx], taskIdx);
        sleep(1);
        if (!forver)
        {
            taskCnt--;
            if (taskCnt == 0)
            {
                break;
            }
        }
    }
    // write quit -> read 0
    for (int i = 0; i < processNum; i++)
    {
        close(subs[i].getsunEpWriteFd());
    }
}

void waitProcess(vector<subEp> &subs)
{
    int processNum = subs.size();
    for (int i = 0; i < processNum; i++)
    {
        waitpid(subs[i].getsubEpId(), nullptr, 0);
        cout << "wait sub process sucess ....：" << subs[i].getsubEpId() << endl;
    }
}

int main()
{
    MAKESEED();
    vector<func_t> funcMap;
    loadTask(&funcMap);
    vector<subEp> subs;
    
    // 1、创建子进程和管道
    creatSubProcess(&subs, funcMap);

    // 2、父进程控制子进程
    int taskCnt = 3; // 0 则永久进行
    loadBalanceCtr(funcMap, subs, taskCnt);

    // 3、回收子进程
    waitProcess(subs);
    return 0;
}