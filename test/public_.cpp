//
// Created by xy on 2023-11-30.
//

#ifndef CONNECTIONPOOL_PUBLIC__CPP
#define CONNECTIONPOOL_PUBLIC__CPP


#include "public_.h"
using namespace std;


// 非连接池
void op1(int begin, int end)
{
    for (int i = begin; i < end; ++i)
    {
        MysqlConn conn;
        conn.connect("127.0.0.1","root", "123", "test");
        char sql[1024] = { 0 };
        snprintf(sql, sizeof(sql), "insert into user values(%d, 'zhang san', '221B')", i);
        conn.update(sql);
    }
}

// 连接池
void op2(ConnectionPool* pool, int begin, int end)
{
    for (int i = begin; i < end; ++i)
    {
        shared_ptr<MysqlConn> conn = pool->getConnection();
        char sql[1024] = { 0 };
        snprintf(sql, sizeof(sql), "insert into user values(%d, 'zhang san', '221B')", i);
        conn->update(sql);
    }
}

#endif //CONNECTIONPOOL_PUBLIC__CPP
