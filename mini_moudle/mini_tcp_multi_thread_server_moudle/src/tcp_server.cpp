#include "tcp_server.h"

#include <algorithm>
#include <string>
#include <thread> 

#include <sys/socket.h>
#include <string.h>
#include <unistd.h>

bool TcpServer::m_online;
int TcpServer::m_lastClosed;
int TcpServer::m_clientNum;
std::mutex TcpServer::m_mt;

char TcpServer::m_msg[MAX_PACKET_SIZE];

std::vector<DESCRIPT_SOCKET_S *> TcpServer::m_newSockFd;
std::vector<DESCRIPT_SOCKET_S *> TcpServer::m_message;

void *TcpServer::Task(void *arg)
{
    pthread_detach(pthread_self());
    DESCRIPT_SOCKET_S *desc = (DESCRIPT_SOCKET_S*)arg;
    int recvByte = 0;
    std::cerr << "open client[ id:"<< desc->id <<" ip:"<< desc->ip <<" socket:"
        << desc->socket <<" send:"<< desc->enableMessageRuntime <<" ]" << std::endl;
    while(1) {
        recvByte = recv(desc->socket, m_msg, MAX_PACKET_SIZE, 0);
        if (recvByte != -1) {
            if (recvByte == 0) {
                m_online = false;
                std::cerr << "close client[ id:"<< desc->id <<" ip:"<< desc->ip <<" socket:"
                    << desc->socket<<" ]" << std::endl;
                m_lastClosed = desc->id;
                close(desc->socket);

                int id = desc->id;
                auto newEnd = std::remove_if(m_newSockFd.begin(), m_newSockFd.end(),
                    [id](DESCRIPT_SOCKET_S *device) {return device->id == id;});
                m_newSockFd.erase(newEnd, m_newSockFd.end());
                if (m_clientNum > 0) {
                    m_clientNum--;
                }
                break;
            }
            m_msg[recvByte] = '\0';
            desc->message = std::string(m_msg);
            std::lock_guard<std::mutex> guard(m_mt);
            m_message.push_back(desc);
        }
        usleep(600);
    }
    usleep(600);
    if (desc != nullptr) {
        delete desc;
        desc = nullptr;
    }
    std::cerr << "exit thread: " << std::this_thread::get_id() << std::endl;
    pthread_exit(nullptr);
}

int TcpServer::setUp(int port, std::vector<int> opts)
{
    m_online = false;
    m_lastClosed = -1;
    m_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (m_socket == -1) {
        std::cerr << "Err: create socket err" << std::endl;
        return -1;
    }
    int opt = 1;
    for (int i = 0; i < opts.size(); i++) {
        if (setsockopt(m_socket, SOL_SOCKET, opts.at(i), (char*)&opt, sizeof(opt)) < 0) {
            std::cerr << "Errore setsockopt" << std::endl; 
            return -1;
        }
    }
    memset(&m_serverAddress, 0, sizeof(m_serverAddress));

    m_serverAddress.sin_family = AF_INET;
    m_serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    m_serverAddress.sin_port = htons(port);
    if (bind(m_socket, (struct sockaddr *)&m_serverAddress, sizeof(m_serverAddress)) < 0) {
        std::cerr << "Errore bind" << std::endl;
        return -1;
    }

    if (listen(m_socket, 5) < 0) {
        std::cerr << "Errore listen" << std::endl;
        return -1;
    }
    m_clientNum = 0;
    m_online = true;

    return 0;
}

void TcpServer::tcpServerAccepted()
{
    socklen_t sockSize = sizeof(m_clientAddress);
    DESCRIPT_SOCKET_S *sock = new DESCRIPT_SOCKET_S;
    sock->socket = accept(m_socket, (struct sockaddr*)&m_clientAddress, &sockSize);
    if (sock->socket == -1) {
        std::cerr << "Errore accept" << std::endl;
        return;
    }
    sock->id = m_clientNum;
    sock->ip = inet_ntoa(m_clientAddress.sin_addr);
    m_newSockFd.push_back(sock);
    std::cout << "accept client[ id:" << m_newSockFd[m_clientNum]->id <<
        " ip:" << m_newSockFd[m_clientNum]->ip << " handle:" << m_newSockFd[m_clientNum]->socket << " ]" << std::endl;
    pthread_create(&m_serverThread[m_clientNum], NULL, &Task, (void *)m_newSockFd[m_clientNum]);
	m_online = true;
	m_clientNum++;
}

void TcpServer::clean(int id)
{
    m_message[id] = nullptr;
    memset(m_msg, 0, MAX_PACKET_SIZE);
}

void TcpServer::sendMsg(std::string msg, int id)
{
    send(m_newSockFd[id]->socket, msg.c_str(), msg.length(), 0);
}

std::vector<DESCRIPT_SOCKET_S*> TcpServer::getMessage()
{
    std::lock_guard<std::mutex> guard(m_mt);
    return m_message;
}

bool TcpServer::isOnline()
{
    return m_online;
}

int TcpServer::getLastClosedSocket()
{
    return m_lastClosed;
}
