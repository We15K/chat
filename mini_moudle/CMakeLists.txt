# cmake 要求的最小版本
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

# 工程名称
PROJECT (mini_moudle)

# 将 mini_server 目录添加到目录树
ADD_SUBDIRECTORY(mini_tcp_multi_thread_server_moudle)

# 将 mini_cJson 目录添加到目录树
ADD_SUBDIRECTORY(mini_cJSON)