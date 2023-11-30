//
// 非连接池, 多单线程, 用时: 7530162947 纳秒, 7530 毫秒
//

#include "public_.h"
#include "MysqlConn.h"
#include <iostream>
#include <thread>
using namespace std;

void test1() {
    MysqlConn conn;
    conn.connect("127.0.0.1","root", "123", "test");
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    std::thread t1(op1, 0, 1000);
    std::thread t2(op1, 1000, 2000);
    std::thread t3(op1, 2000, 3000);
    std::thread t4(op1, 3000, 4000);
    std::thread t5(op1, 4000, 5000);
    t1.join();
    t2.join();
    t3.join();
    t4.join();
    t5.join();
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    auto length = end - begin;
    cout << "非连接池, 多单线程, 用时: " << length.count() << " 纳秒, "
        << length.count() / 1000000 << " 毫秒" << endl;
}

#if 0

int main(){
    test1();
    return 0;
}

#endif