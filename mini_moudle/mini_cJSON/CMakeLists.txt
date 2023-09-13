# cmake 要求的最小版本
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

# 将 头文件 加入搜索目录
INCLUDE_DIRECTORIES(inc)

SET(SRC_FILES src/cJSON.c)

SET(LIB_NAME cJSON)

# 添加动态库的构建规则
ADD_LIBRARY(${LIB_NAME} SHARED ${SRC_FILES})

# 指定动态库和头文件安装路径
SET(INSTALL_PATH /home/mengc/local)

# 将头文件安装到指定位置
INSTALL(FILES inc/cJSON.h DESTINATION ${INSTALL_PATH}/inc)

# 将动态库文件安装到指定位置
INSTALL(TARGETS ${LIB_NAME} LIBRARY DESTINATION ${INSTALL_PATH}/lib)

# 将 test 目录添加到目录树
ADD_SUBDIRECTORY(test)