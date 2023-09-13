# cmake 要求的最小版本
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

# 需要按顺序添加，否则可能出现因为编译先后问题导致的对象成员函数无法找到
SET(SRC_LIST cJSON_test.cpp)

# 生成可以gdb的进程
ADD_DEFINITIONS("-Wall -g")

# 添加生成二进制与依赖
ADD_EXECUTABLE(cJSON.out ${SRC_LIST})

# 链接动态库
TARGET_LINK_LIBRARIES(cJSON.out cJSON)