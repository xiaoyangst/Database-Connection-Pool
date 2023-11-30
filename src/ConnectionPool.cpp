#include "ConnectionPool.h"
#include "../thirdparty/json.hpp"

using json = nlohmann::json;

#include <fstream>
#include <thread>

ConnectionPool::ConnectionPool() {
    parseJsonFile();    //先解析json配置文件

    for (int k = 0; k < minSize_; ++k) {    //创建空闲数据池
        addConnection();
    }

    std::thread producer(&ConnectionPool::produceConnection, this);
    std::thread recycler(&ConnectionPool::recycleConnection, this);

    producer.detach();
    recycler.detach();
}

ConnectionPool::~ConnectionPool() {
    while (!connectionQueue_.empty()) {
        MysqlConn *conn = connectionQueue_.front();
        connectionQueue_.pop();
        delete conn;
    }
}

//解析配置文件
bool ConnectionPool::parseJsonFile() {
    std::string path = "./source/conf.json";
    std::ifstream file(path);
    if (!file.is_open()) return false;
    json conf = json::parse(file);  //读取一个json文件得到一个json对象
    ip_ = conf.at("ip");
    user_ = conf.at("username");
    passwd_ = conf.at("password");
    dbName_ = conf.at("dbName");
    port = conf.at("port");
    minSize_ = conf.at("minSize");
    maxSize_ = conf.at("maxSize");
    timeout_ = conf.at("timeout");
    maxIdleTime_ = conf.at("maxIdleTime");
    return true;
}

void ConnectionPool::addConnection() {
    MysqlConn *conn = new MysqlConn;
    conn->connect(ip_, user_, passwd_, dbName_, port);
    conn->refreshAliveTime();   //实际上就是记录着连接最初创建时候的起始时间
    connectionQueue_.push(conn);
}

//生产者模块
void ConnectionPool::produceConnection() {
    //调用一次就循环创建连接池，如果达到连接池最高限度就阻塞（wait）
    while (true) {
        std::unique_lock<std::mutex> lock(mutex_);
        while (connectionQueue_.size() >= maxSize_) {
            cond_.wait(lock);
        }
        addConnection();
        cond_.notify_all();
    }
}

//回收多余的连接
void ConnectionPool::recycleConnection() {
    while (true) {
        //周期性检查
        std::this_thread::sleep_for(std::chrono::microseconds(500));
        std::unique_lock<std::mutex> lock(mutex_);
        while (connectionQueue_.size() > minSize_) {        //保持在最低限度连接池数量，但如果这个连接状态并非长时间没有被使用的话并不会回收
            MysqlConn *conn = connectionQueue_.front();
            if (conn->getAliveTime() >= maxIdleTime_) {  //连接未被使用时间过长，可以销毁
                connectionQueue_.pop();
                delete conn;
            } else {
                break;
            }
        }
    }
}

std::shared_ptr<MysqlConn> ConnectionPool::getConnection() {
    std::unique_lock<std::mutex> lock(mutex_);
    //这个设计巧妙，如果连接池为空可以等待一会儿
    while (connectionQueue_.empty()) {
        //等待期间未收到通知而发生了超时
        if (cond_.wait_for(lock, std::chrono::milliseconds(timeout_)) == std::cv_status::timeout) {
            //超时就不能再等，否则就是无意义的死循环和阻塞
            if (connectionQueue_.empty()) continue;
        }
    }
    //现在已经来到能够获取有效连接阶段
    //但要回答一个问题：用完连接之后，如何归还呢？
    //使用 std::shared_ptr 来管理 MysqlConn 对象，然后自定义删除器
    //这个删除器的作用是在 std::shared_ptr 对象引用计数减为零时被调用，从而在释放资源之前执行一些自定义的操作
    std::shared_ptr<MysqlConn> connptr(connectionQueue_.front(),
                                       [this](MysqlConn *conn) {
                                           std::lock_guard<std::mutex> locker(mutex_);
                                           conn->refreshAliveTime();
                                           connectionQueue_.push(conn);
                                       });
    connectionQueue_.pop();
    cond_.notify_all();
    return connptr;
}