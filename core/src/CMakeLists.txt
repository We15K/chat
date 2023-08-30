# cmake 要求的最小版本
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

# 需要按顺序添加，否则可能出现因为编译先后问题导致的对象成员函数无法找到
SET(SRC_LIST main.cpp)

# 添加生成二进制与依赖
ADD_EXECUTABLE(chat_server ${SRC_LIST})