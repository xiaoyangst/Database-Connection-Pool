//
// 非连接池, 单线程, 用时: 25640095111 纳秒, 25640 毫秒
//

#include "public_.h"
#include <iostream>
#include <thread>
using namespace std;

void test3()
{
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    op1(0, 5000);
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    auto length = end - begin;
    cout << "非连接池, 单线程, 用时: " << length.count() << " 纳秒, "
        << length.count() / 1000000 << " 毫秒" << endl;

}

#if 0

int main(){
    test3();
    return 0;
}

#endif