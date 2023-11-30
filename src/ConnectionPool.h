/*
 * 外部使用数据库连接池调用的头文件
 */

#ifndef SQLCONPOOL_CONNECTIONPOOL_H
#define SQLCONPOOL_CONNECTIONPOOL_H

#include "noncopyable.h"
#include "MysqlConn.h"

#include <queue>    //存放连接的容器
#include <memory>
#include <mutex>
#include <condition_variable>

class ConnectionPool : noncopyable{
public:
    static ConnectionPool* getInstance(){
        static ConnectionPool instance;
        return &instance;
    }

    ConnectionPool();
    ~ConnectionPool();
    std::shared_ptr<MysqlConn> getConnection();

private:
    bool parseJsonFile();        //MySQL配置信息解析（Json格式）
    void produceConnection();
    void recycleConnection();
    void addConnection();

    std::string ip_;
    std::string user_;
    std::string passwd_;
    std::string dbName_;
    unsigned short port;

    int minSize_;
    int maxSize_;
    int timeout_;
    int maxIdleTime_;

    std::queue<MysqlConn*> connectionQueue_;
    std::mutex mutex_;
    std::condition_variable cond_;
};

#endif //SQLCONPOOL_CONNECTIONPOOL_H
