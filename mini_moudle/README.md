# MINI_MOUDLE

## 说明

- 采用小组件 -> 组合服务器 模块化开发
- chat server的小组件
- 主要包含以下几个模块

### Moudle 1	tcp多线程server

**作用**：

- 接收新的tcp链接
- socket链接的存储，初步id的分配
- 消息的接收
- 消息的发送
- 业务相关线程的创建

### Moudle 2	客户端信息管理

- 用户身份验证
- 根据登录的账户信息生成用户身份uuid
- 存储登录信息和用户身份uuid
- 根据登录信息匹配用户身份uuid
- 用户在线状态管理

![Image text](https://github.com/We15K/chat/blob/main/mini_moudle/assets/%E7%A7%AF%E6%9C%A8.png)
