//
// 测试数据库连接性并进行查询
//

#include <iostream>
#include <memory>
#include "../src/MysqlConn.h"
using namespace std;

int query()
{
    MysqlConn conn;
    conn.connect("127.0.0.1","root", "123", "test");
    string sql = "insert into user values(1, 'zhang san', '221B')";
    bool flag = conn.update(sql);
    cout << "flag value:  " << flag << endl;

    sql = "select * from user";
    conn.query(sql);
    // 从结果集中取出一行
    while (conn.next())
    {
        // 打印每行字段值
        cout << conn.value(0) << ", "
             << conn.value(1) << ", "
             << conn.value(2) << ", "
             << conn.value(3) << endl;
    }
    return 0;
}

#if 1

int main(){
    query();
    return 0;
}

#endif