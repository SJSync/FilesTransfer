#include <iostream>
#include <fstream>
#include <winsock2.h> 
#pragma comment(lib,"ws2_32.lib")

#define BUFSIZE 1024

using namespace std;

class Socket
{
private:
    WORD wVersionRequested;
    WSADATA wsaData;
    SOCKET sockfd, s_socket;
    struct sockaddr_in server;
    struct sockaddr_in tcpAddr;
    char buf[BUFSIZE];
    int rval = 0;
    ofstream ofs;

public:
    char file[BUFSIZE];
    char clientIp[128];
    int clientPort;

public:
    Socket(const int port);
    ~Socket();
    bool work();
    bool recvFile(const char filename[]);
};