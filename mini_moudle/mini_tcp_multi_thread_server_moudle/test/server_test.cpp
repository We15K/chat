#include "tcp_server.h"

#include <ctime>

#include <unistd.h>

int messageNum;
int sendTime = 2700;
TcpServer tcpServer;
pthread_t msgHandle[MAX_CLIENT];

void *TcpSendToClient(void *arg)
{
    DESCRIPT_SOCKET_S *desc = (DESCRIPT_SOCKET_S*)arg;
    while(1) {
        if (!tcpServer.isOnline() && tcpServer.getLastClosedSocket() == desc->id) {
            std::cerr << "Connection has been disconnected: stop send_clients( id:" << desc->id << " ip:" << desc->ip << " )"<< std::endl;
            break;
        }

        std::time_t t = std::time(0);
        std::tm* now = std::localtime(&t);
        int hour = now->tm_hour;
		int min  = now->tm_min;
		int sec  = now->tm_sec;
        std::string date = 
            std::to_string(now->tm_year + 1900) + "-" +
            std::to_string(now->tm_mon + 1)     + "-" +
            std::to_string(now->tm_mday)        + " " +
            std::to_string(hour)                + ":" +
            std::to_string(min)                 + ":" +
            std::to_string(sec)                 + "\r\n";
        std::cout << date << std::endl;
        tcpServer.sendMsg(date, desc->id);
        sleep(sendTime);
    }
    pthread_exit(NULL);
    return 0;
}

void *TcpServerReceived(void *arg)
{
    pthread_detach(pthread_self());

    std::vector<DESCRIPT_SOCKET_S*> desc;
    while(1) {
        desc = tcpServer.getMessage();
        for (unsigned int i = 0; i < desc.size(); i++) {
            if (desc[i]) {
                if (!desc[i]->enableMessageRuntime) {
                    desc[i]->enableMessageRuntime = true;
                    if (pthread_create(&msgHandle[messageNum], nullptr, TcpSendToClient, (void*)desc[i]) == 0) {
                        std::cout << "Active thraed process messaging" << std::endl;
                    }
                    std::cout << "id: " << desc[i]->id << std::endl
                        << "ip: " << desc[i]->ip << std::endl
                        << "message: " << desc[i]->message << std::endl
                        << "socket: " << desc[i]->socket << std::endl
                        << "enable: " << desc[i]->enableMessageRuntime << std::endl;
				    tcpServer.clean(i);
                }
            }
        }
        usleep(1000);
    }

    return nullptr;
}

int main(int argc, char **argv)
{
    std::cout << "Hello tcp server" << std::endl;

    if (argc < MINI_AERC) {
        std::cerr << "Usage: ./server port (opt)time-send" << std::endl;
        return -1;
    }
    if (argc == THREE_AERC) {
        sendTime = atoi(argv[2]);
    }
    pthread_t msg;
    std::vector<int> opts = { SO_REUSEPORT, SO_REUSEADDR };
    if (tcpServer.setUp(atoi(argv[1]), opts) == 0) {
        if (pthread_create(&msg, nullptr, TcpServerReceived, (void *)0) == 0) {
            while(1) {
                tcpServer.tcpServerAccepted();
                std::cerr << "Accepted" << std::endl;
            }
        }
    } else {
        std::cerr << "Errore apertura socket" << std::endl;
    }

    return 0;
}