//
// Created by xy on 2023-11-30.
//

#ifndef CONNECTIONPOOL_PUBLIC__H
#define CONNECTIONPOOL_PUBLIC__H

#include <iostream>
#include <memory>
#include <thread>
#include "../src/MysqlConn.h"
#include "../src/ConnectionPool.h"

void op1(int begin, int end);
void op2(ConnectionPool* pool, int begin, int end);

#endif //CONNECTIONPOOL_PUBLIC__H
