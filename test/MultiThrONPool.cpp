//
// 连接池, 多单线程, 用时: 466447317 纳秒, 466 毫秒
//
#include "public_.h"
#include "ConnectionPool.h"
#include <iostream>
#include <thread>
using namespace std;

void test2()
{
    ConnectionPool* pool = ConnectionPool::getInstance();
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    std::thread t1(op2, pool, 0, 1000);
    std::thread t2(op2, pool, 1000, 2000);
    std::thread t3(op2, pool, 2000, 3000);
    std::thread t4(op2, pool, 3000, 4000);
    std::thread t5(op2, pool, 4000, 5000);
    t1.join();
    t2.join();
    t3.join();
    t4.join();
    t5.join();
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    auto length = end - begin;
    cout << "连接池, 多单线程, 用时: " << length.count() << " 纳秒, "
         << length.count() / 1000000 << " 毫秒" << endl;

}

#if 0

int main(){
    test2();
    return 0;
}

#endif