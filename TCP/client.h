#include <iostream>
#include <fstream>
#include <string>
#include <winsock2.h> 
#pragma comment(lib,"ws2_32.lib")

#define BUFSIZE 1024


class Client
{
private:
    WORD wVersionRequested;
    WSADATA wsaData;
    SOCKET c_socket;
    struct sockaddr_in serverAddr;
    char buf[BUFSIZE];
    int rval = 0;
    std::ifstream ifs;

public:
    size_t fileSize;
    size_t sentBytes;
    std::string name;

public:
    Client(const char addr[], const int port);
    ~Client();
    bool work(const char filename[]);
    bool sendFile(const char filename[]);
};