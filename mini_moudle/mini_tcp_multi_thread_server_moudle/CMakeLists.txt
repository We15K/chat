# cmake 要求的最小版本
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

# 将 头文件 加入搜索目录
INCLUDE_DIRECTORIES(inc)

# 需要按顺序添加，否则可能出现因为编译先后问题导致的对象成员函数无法找到
SET(SRC_LIST src/tcp_server.cpp test/server_test.cpp)

# 生成可以gdb的进程
ADD_DEFINITIONS("-Wall -g")

# 添加生成二进制与依赖
ADD_EXECUTABLE(tcp_multi_thread_server ${SRC_LIST})