#pragma once

#include <iostream>
#include <vector>
#include <mutex>

#include <arpa/inet.h>
#include <pthread.h>

#define MINI_AERC 2
#define THREE_AERC 3
#define ILLEGALITY_SOCKET -1
#define MAX_CLIENT 1000
#define MAX_PACKET_SIZE 40960

typedef struct DESCRIPT_SOCKET {
    int socket  = -1;
    std::string ip = "";
    int id = -1; 
    std::string message;
    bool enableMessageRuntime = false;
} DESCRIPT_SOCKET_S;

class TcpServer {
public:
    void tcpServerAccepted();
    int setUp(int port, std::vector<int> opts);
    std::vector<DESCRIPT_SOCKET_S*> getMessage();
    bool isOnline();
    int getLastClosedSocket();
    void sendMsg(std::string msg, int id);
    void clean(int id);
private:
    int m_socket;
    struct sockaddr_in m_serverAddress;
    struct sockaddr_in m_clientAddress;
    pthread_t m_serverThread[MAX_CLIENT];

    static bool m_online;
    static int m_lastClosed;
    static int m_clientNum;
    static char m_msg[MAX_PACKET_SIZE];
    static std::mutex m_mt;

    static std::vector<DESCRIPT_SOCKET_S *> m_newSockFd;
    static std::vector<DESCRIPT_SOCKET_S *> m_message;

    static void *Task(void *arg);
};