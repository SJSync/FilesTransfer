#ifndef _SERVER_H
#define _SERVER_H

#include <iostream>
#include <fstream>
#include <chrono>
#include <winsock2.h> 
#pragma comment(lib,"ws2_32.lib")

#define BUFSIZE 1024

class Server
{
private:
    WORD wVersionRequested;
    WSADATA wsaData;
    SOCKET sockfd, s_socket;
    struct sockaddr_in server;
    struct sockaddr_in tcpAddr;
    char buf[BUFSIZE];
    int rval = 0;
    std::ofstream ofs;

public:
    char file[BUFSIZE];
    char clientIp[128];
    int clientPort;

public:
    size_t fileSize;
    size_t recvSize;
    double time;

public:
    Server(const int port);
    ~Server();
    bool work();
    bool recvFile(const char filename[], double& time);
};

#endif
