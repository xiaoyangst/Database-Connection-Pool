已经把测试用例写在test文件夹下面，只需要开启需要测试的源文件即可

```cmake
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug

cmake --build build
```

可执行文件将会在bin文件夹下面生成

<span style="color:red">注意：</span>修改conf.json配置文件中的账号和密码为你的，必须是导入source下的sql文件到数据库中测试方可进行

------

学习收获：

1. C++11多线程安全单例模式
2. 智能指针
3. 生产者-消费者模型
4. 互斥锁和同步变量
5. 封装API的思想

