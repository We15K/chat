# cmake 要求的最小版本
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

# 工程名称
PROJECT (chat_server)

# 在 opensource下查找Cmakelists.txt并执行
# ADD_SUBDIRECTORY(opensource)

# 将 头文件 加入搜索目录
INCLUDE_DIRECTORIES(inc)

# 将 src 目录添加到目录树
ADD_SUBDIRECTORY(src bin)