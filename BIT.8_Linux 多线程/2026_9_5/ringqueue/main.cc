#include "RingQueue.hpp"
#include "task.hpp"
#include <ctime>
#include <cstdlib>

#include <pthread.h>
#include <unistd.h>

std::string SelfName()
{
    char name[128];
    snprintf(name, sizeof name, "thead-0x%lx", pthread_self());
    return name;
}

void *ProducerStartRoutine(void *rq)
{
    RingQueue<Task> *ringqueue = (RingQueue<Task> *)rq;
    while (1)
    {
        // V-1
        // int data = rand() % 10000 + 1;
        // ringqueue->Push(data);
        // std::cout << "生产完成，生产的数据是：" << data << std::endl;

        // V-2
        // 构建或者是获取任务
        int x = rand() % 10;
        int y = rand() % 5;
        char op = oper[rand() % oper.size()];
        Task t(x, y, op, myop);
        // 生产任务
        ringqueue->Push(t);

        // 输出提示
        std::cout << SelfName() << " 生产者派发了一个任务" << t.toTaskString() << std::endl;

        // sleep(1);
    }
    return nullptr;
}

void *ConsumerStartRoutine(void *rq)
{
    RingQueue<Task> *ringqueue = (RingQueue<Task> *)rq;
    while (1)
    {
        // V-1
        // int data;
        // ringqueue->Pop(&data);
        // std::cout << "消费完成，消费的数据是：" << data << std::endl;
        // // sleep(1);

        // V-2
        // 获取任务
        Task t;
        ringqueue->Pop(&t);

        // 消费任务

        std::string result = t();

        // 输出提示
        std::cout << SelfName() << " 消费者消费了一个任务" << result << std::endl;

        // sleep(1);
    }
    return nullptr;
}

int main()
{
    srand((u_int32_t)time(nullptr) ^ getpid() ^ pthread_self() ^ 0x777777); // 植入种子

    RingQueue<Task> *rq = new RingQueue<Task>(5); // 一种共享资源

    // pthread_t consumer, producer;                  // 两类角色

    std::vector<pthread_t> consumer(5), producer(8); // 两类角色

    for (auto &e : producer)
    {
        assert(pthread_create(&e, nullptr, ProducerStartRoutine, (void *)rq) == 0);
    }

    for (auto &e : consumer)
    {
        assert(pthread_create(&e, nullptr, ConsumerStartRoutine, (void *)rq) == 0);
    }

    // 单生产单消费
    // pthread_create(&producer, nullptr, ProducerStartRoutine, (void *)rq);

    // pthread_create(&consumer, nullptr, ConsumerStartRoutine, (void *)rq);

    // 多生产多消费

    for (auto &e : producer)
    {
        pthread_join(e, nullptr);
    }

    for (auto &e : consumer)
    {
        pthread_join(e, nullptr);
    }

    // pthread_join(producer, nullptr);

    // pthread_join(consumer, nullptr);

    delete rq;

    return 0;
}