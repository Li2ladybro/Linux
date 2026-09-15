// 包含生产者和消费者

#include "BlockQueue.hpp"
#include "task.hpp"

#include <ctime>

#include <unistd.h>

// C:计算，S:保存
template <class C, class S>
struct BlockQueues
{
    BlockQueue<C> *c_bq;
    BlockQueue<S> *s_bq;
};

void *producer_start_routine(void *_bqs)
{
    BlockQueue<CalTask> *bq = ((BlockQueues<CalTask, SaveTask> *)_bqs)->c_bq;
    while (1)
    {
        // 生产活动  从数据库，从网络，从。。。。获取任务
        int x = rand() % 100 + 1; // 构建一个随机数据
        int y = rand() % 10 + 1;  // 构建一个随机数据
        int operCode = rand() % oper.size();

        // 构建任务
        CalTask t(x, y, oper[operCode], myop); // 耗时

        bq->push(t);
        std::cout << "producor thread 生产计算任务: " << t.toTaskString() << std::endl;
        sleep(1);
    }
    return nullptr;
}

void *consumer_start_routine(void *_bqs)
{
    BlockQueue<CalTask> *cal_bq = ((BlockQueues<CalTask, SaveTask> *)_bqs)->c_bq;
    BlockQueue<SaveTask> *save_bq = ((BlockQueues<CalTask, SaveTask> *)_bqs)->s_bq;

    while (1)
    {
        // 1、消费活动
        CalTask t;
        cal_bq->pop(&t); // 拿出任务

        // 处理任务
        std::string result = t(); // 非常耗时
        std::cout << "consumer thread 完成计算任务: " << result << " .....done" << std::endl;

        // 2、将任务结果推送到保存阻塞队列
        SaveTask save(result, Save);
        save_bq->push(save);
        std::cout << "consumer thread 推送保存任务完成....\n ";
    }
    return nullptr;
}

void *save_task_start_routine(void *_bqs)
{
    // 处理保存阻塞队列任务
    BlockQueue<SaveTask> *save_bq = ((BlockQueues<CalTask, SaveTask> *)_bqs)->s_bq;

    while (1)
    {
        SaveTask t;
        save_bq->pop(&t);
        t();

        std::cout << "save thread保存任务完成....\n ";
    }
    return nullptr;
}

int main()
{
    srand((unsigned long)(nullptr) ^ getpid());

    BlockQueues<CalTask, SaveTask> *bqs = new BlockQueues<CalTask, SaveTask>;

    bqs->c_bq = new BlockQueue<CalTask>();  // 任务阻塞队列：同一份资源，缓冲区
    bqs->s_bq = new BlockQueue<SaveTask>(); // 保存阻塞队列：同一份资源，缓冲区

    pthread_t consumer[5], producer[5]; // 生产消费者任务线程

    for (int i = 0; i < 5; ++i)
    {

        pthread_create(consumer + i, nullptr, consumer_start_routine, (void *)bqs);
    }
    for (int i = 0; i < 5; ++i)
    {
        pthread_create(producer + i, nullptr, producer_start_routine, (void *)bqs);
    }

    pthread_t save_thread; // 保存线程
    pthread_create(&save_thread, nullptr, save_task_start_routine, (void *)bqs);

    for (int i = 0; i < 5; ++i)
    {
        pthread_join(consumer[i], nullptr);
        pthread_join(producer[i], nullptr);
    }
    pthread_join(save_thread, nullptr);

    delete bqs->c_bq;
    delete bqs->s_bq;
    delete bqs;

    return 0;
}
