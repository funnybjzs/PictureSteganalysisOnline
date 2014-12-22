#ifndef TCPSOCKET_H
#define TCPSOKET_H
#include <unistd.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "Recieve.h"

#define MAXSIZE 1024000

class TcpServer
{
private:
        int socket_fd,client_fd;   //服务器、客户端套接字
        sockaddr_in myserver;      //服务器网络地址结构
        sockaddr_in remote_addr;   //客户端网络地址结构

public:
        TcpServer(int listenPort);
        bool Accpet();
        void RecvMsg();
        void Start();
        void Stop();
        void Close();
};
#endif
