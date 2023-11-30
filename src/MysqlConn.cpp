#include "MysqlConn.h"

MysqlConn::MysqlConn() {
    conn_ = mysql_init(nullptr);    //初始化MySQL对象
    mysql_set_character_set(conn_,"utf8");  //设置字符编码为utf-8
}

bool   //本质上进行连接测试，并不返回实际连接的对象
MysqlConn::connect(const std::string &ip, const std::string &user, const std::string &passwd, const std::string dbName,
                   const unsigned int &port) {
    if (mysql_real_connect(conn_,ip.c_str(),user.c_str(),passwd.c_str(),dbName.c_str(),port, nullptr,0)) return true;
    return false;
}

MysqlConn::~MysqlConn() {
    if (conn_ != nullptr){
        mysql_close(conn_);     //关闭MySQL连接
    }
    freeResult();       //释放结果集
}

bool MysqlConn::update(const std::string &sql) {
    if (mysql_query(conn_,sql.c_str()) == 0) return true;
    return false;
}

bool MysqlConn::query(const std::string &sql) {
    freeResult();   // 查询前确保结果集为空
    if (mysql_query(conn_,sql.c_str()) == 0){
        result_ = mysql_store_result(conn_);    //获取查询结果集
    } else{
        return false;
    }
    return true;
}

bool MysqlConn::next() {
    if (result_ == nullptr) return false;
    row_ = mysql_fetch_row(result_);    //获取结果集中的一行
    if (row_ == nullptr) return false;
    return true;
}

std::string MysqlConn::value(int index) {
    //获取字段数
    int rowCount = mysql_num_fields(result_);
    if (index >= rowCount || index < 0) return std::string();
    //考虑到存储结果可能为二进制字符串，包含'\0'
    //先获取字符串头指针和字符串长度，能够得到完整的字符串
    char* val = row_[index];    //字段值的起始位置
    unsigned long length = mysql_fetch_lengths(result_)[index];
    return std::string(val,length);
}


void MysqlConn::refreshAliveTime() {
    m_alivetime = std::chrono::steady_clock::now();
}

//获取连接存活时长，如果长时间灭有被使用，应该选择被释放掉
long long MysqlConn::getAliveTime() {
    std::chrono::nanoseconds res = std::chrono::steady_clock::now() - m_alivetime;
    std::chrono::milliseconds milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(res);
    return milliseconds.count();
}

// 事务操作
bool MysqlConn::transaction()
{
    // true  自动提交
    // false 手动提交
    return mysql_autocommit(conn_, false);
}

// 提交事务
bool MysqlConn::commit()
{
    return mysql_commit(conn_);
}

// 事务回滚
bool MysqlConn::rollbock()
{
    return mysql_rollback(conn_);
}

void MysqlConn::freeResult() {
    if (result_)
    {
        mysql_free_result(result_);
        result_ = nullptr;
    }
}
