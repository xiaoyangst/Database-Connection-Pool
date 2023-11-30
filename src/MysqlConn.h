/*
 * 封装常用数据库编程API
 */

#ifndef SQLCONPOOL_MYSQLCONN_H
#define SQLCONPOOL_MYSQLCONN_H

#include <mysql/mysql.h>
#include <iostream>
#include <chrono>

class MysqlConn
{
public:
    // 初始化数据库连接
    MysqlConn();
    // 释放数据库连接
    ~MysqlConn();
    // 连接数据库
    bool connect(const std::string& ip, const std::string& user, const std::string& passwd, const std::string dbName, const unsigned int& port = 3306);
    // 更新数据库：包括 insert update delete 操作
    bool update(const std::string& sql);
    // 查询数据库
    bool query(const std::string& sql);
    // 遍历查询得到的结果集
    bool next();
    // 得到结果集中的字段值
    std::string value(int index);
    // 事务操作
    bool transaction();
    // 提交事务
    bool commit();
    // 事务回滚
    bool rollbock();
    // 刷新起始的空闲时间点
    void refreshAliveTime();
    // 计算连接存活的总时长
    long long getAliveTime();

private:
    void freeResult();

    MYSQL* conn_ = nullptr;     //MySQL连接句柄，就像你要文件操作前必须FILE一个文件描述符，这必然是第一步
    MYSQL_RES* result_ = nullptr;   //执行sql返回的结果（通常存储查询结果）
    MYSQL_ROW row_ = nullptr;       //遍历result_用得到

    // 绝对时钟
    std::chrono::steady_clock::time_point m_alivetime;
};

#endif //SQLCONPOOL_MYSQLCONN_H
