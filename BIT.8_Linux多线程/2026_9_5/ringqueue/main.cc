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

    // 多生产多消费，需要同步锁和互斥锁
    // 生产者与生产者互斥（多个生产者不可以同时往队列中放数据）：需要生产者互斥锁
    // 消费者与消费者互斥（多个消费者不可以同时往队列中拿数据）：需要消费者互斥锁
    // 生产者与消费者的互斥同步关系（队列为空此时只能生产者工作，队列满只能此时只能消费者工作）：需要同步锁

    // 意义在哪里：生产者构建或者是消费者获取任务处理任务--是需要花费时间的

    for (auto &e : producer)
    {
        assert(pthread_create(&e, nullptr, ProducerStartRoutine, (void *)rq) == 0);
    }

    for (auto &e : consumer)
    {
        assert(pthread_create(&e, nullptr, ConsumerStartRoutine, (void *)rq) == 0);
    }

    // 单生产单消费，只需要一把同步锁就可以保证消费者与生产者的互斥关系
    // 生产者与生产者
    // 消费者与消费者
    // 生产者与消费者的互斥同步关系：只体现在队列为空或者满，此时的同步锁就可以使得两者互斥的访问资源

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