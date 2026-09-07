#include "ThreadPool.hpp"
#include "Thread.hpp"
#include "task.hpp"

#include <memory>

#include <unistd.h>

// void* run(void* args)
// {
//     std::string message = (const char *)args;
//     while (1)
//     {
//         std::cout<<message<<std::endl;
// sleep(1);
//     }
//     return nullptr;
// }

int main()
{
  // 一堆代码。。。



  // ThreadNs::Thread t1(run,(void*)"thread 1 run...");
  // ThreadNs::Thread t2(run,(void*)"thread 2 run...");
  // t1.start();
  // t2.start();
  // t1.join();
  // t2.join();

  // std::unique_ptr<ThreadPool<Task>> tp(new ThreadPool<Task>());

  // 懒汉单例模式，延迟加载
  ThreadPool<Task>::getInstance()->run();
  // tp->run();

  int x, y;
  char op;

  while (1)
  {
    std::cout << "请输入数据1# ";
    std::cin >> x;

    std::cout << "请输入操作运算符# ";
    std::cin >> op;

    std::cout << "请输入数据2# ";
    std::cin >> y;

    Task t(x, y, op, myop);

    ThreadPool<Task>::getInstance()->push(t);

    // std::cout << "你刚才录入了一个任务：" << t.toTaskString() << "，确认提交吗?【y/n】# ";
    // char confirm;
    // std::cin >> confirm;
    // if (confirm == 'y')
    // {
    //   tp->push(t);
    //   // std::cout<<"推送任务成功\n";
    //   fflush(stdout);
    //   sleep(1);
    // }
    // else
    // {
    //   // do nothing
    // }
  }

  return 0;
}
