# cmake 要求的最小版本
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

# 将 头文件 加入搜索目录
INCLUDE_DIRECTORIES(inc)

# 在 opensource下查找Cmakelists.txt并执行
ADD_SUBDIRECTORY(src bin)