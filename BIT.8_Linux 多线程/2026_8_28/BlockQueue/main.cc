// 包含生产者和消费者

#include "BlockQueue.hpp"

#include <ctime>

#include <unistd.h>

void *consumer_start_routine(void *_bq)
{
    BlockQueue<int> *bq = (BlockQueue<int> *)_bq;
    while (1)
    {
        // 消费活动
        int data;
        bq->pop(&data);
        std::cout << "消费数据: " << data << std::endl;
        sleep(1);

    }
    return nullptr;
}

void *producer_start_routine(void *_bq)
{
    BlockQueue<int> *bq = (BlockQueue<int> *)_bq;
    while (1)
    {
        // 生产活动
        int data = rand() & 10 + 1; // 构建一个随机数据
        bq->push(data);
        std::cout << "生产数据: " << data << std::endl;
        // sleep(1);
    }
    return nullptr;
}
int main()
{
    srand((unsigned long)(nullptr) ^ getpid());

    BlockQueue<int> *bq = new BlockQueue<int>(); // 阻塞队列：同一份资源，缓冲区

    pthread_t consumer, producer; // 生产消费者线程

    pthread_create(&consumer, nullptr, consumer_start_routine, (void *)bq);
    pthread_create(&producer, nullptr, producer_start_routine, (void *)bq);

    pthread_join(consumer, nullptr);
    pthread_join(producer, nullptr);

    delete bq;

    return 0;
}