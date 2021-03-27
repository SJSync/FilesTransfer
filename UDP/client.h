#include <iostream>
#include <fstream>
#include <string>
#include <winsock2.h> 
#pragma comment(lib,"ws2_32.lib")

#define BUFSIZE 1024


class Client
{
private:
    // socket版本
    WORD wVersionRequested;
    WSADATA wsaData;
    // 描述服务器的套接字
    SOCKET c_socket;
    struct sockaddr_in serverAddr;
    char buf[BUFSIZE];
    // 接受反馈
    int rval = 0;
    // 文件流指针，用于打开文件
    std::ifstream ifs;

public:
    // 描述文件大小
    size_t fileSize;
    // 描述已发送的字节
    size_t sentBytes;
    std::string name;

public:
    Client(const char addr[], const int port);
    ~Client();
    // TCP连接建立以及相关信息传输
    bool work(const char filename[]);

private:
    // 通过套接字发送文件
    bool sendFile(const char filename[]);
};