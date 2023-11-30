//
// 连接池, 单线程, 用时: 715399459 纳秒, 715 毫秒
//
#include "public_.h"
#include "ConnectionPool.h"
#include <iostream>
#include <thread>

using namespace std;

void test4()
{

    ConnectionPool* pool = ConnectionPool::getInstance();
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    op2(pool, 0, 5000);
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    auto length = end - begin;
    cout << "连接池, 单线程, 用时: " << length.count() << " 纳秒, "
         << length.count() / 1000000 << " 毫秒" << endl;
}

#if 0

int main(){
    test4();
    return 0;
}

#endif