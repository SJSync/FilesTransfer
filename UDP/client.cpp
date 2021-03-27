#include "client.h"


Client::Client(const char addr[], const int port)
{
    fileSize = 0;
    sentBytes = 0;

    wVersionRequested = MAKEWORD(2, 2);
    WSAStartup(wVersionRequested, &wsaData);
    if ((c_socket = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET)
    {
        std::cout << "Can not create socket!" << std::endl;
        exit(0);
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.S_un.S_addr = inet_addr(addr);
    memset(serverAddr.sin_zero, 0, sizeof(serverAddr.sin_zero));
}

Client::~Client()
{
    if(c_socket != INVALID_SOCKET)
    {
        closesocket(c_socket);
    }
    WSACleanup();
}

bool Client::work(const char path[])
{
    int addr_len = sizeof(struct sockaddr_in);

    //
    name = path;
    size_t lastPos = name.find_last_of("/\\");
    name = name.substr(lastPos + 1);
	char* filename = const_cast<char*>(name.c_str());

    //向服务器发送文件名
    strcpy(buf, filename);
    sendto(c_socket, buf, BUFSIZE, 0,(struct sockaddr*)&serverAddr,sizeof(serverAddr) );

    //接收服务器返回确认
    recvfrom(c_socket, buf, BUFSIZE, 0,(struct sockaddr*)&serverAddr,&addr_len);
    if(strcmp(buf, "recv") == 0)
    {
        if(!sendFile(path))
        {
            return false;
        }
        closesocket(c_socket);
    }
    return true;
}

bool Client::sendFile(const char path[])
{
    int readLen = 0;
    sentBytes = 0;

    //打开发送的文件
    ifs.open(path, std::ios::in | std::ios::binary);
    if(!ifs.is_open())
    {
        std::cout << "Can not open " << path << std::endl;
        exit(0);
    }

    //获取文件大小
    ifs.seekg(0, std::ios::end);
    fileSize = ifs.tellg();
    ifs.seekg(0, std::ios::beg);

    //发送文件
    memset(buf, 0, sizeof(buf));
    while(!ifs.eof())
    {
        ifs.read(buf, BUFSIZE);
        readLen = ifs.gcount();
        sentBytes += readLen;
        if(sendto(c_socket, buf, readLen, 0,(struct sockaddr*)&serverAddr,sizeof(serverAddr)) == -1)
        {
            ifs.close();
            return false;
        }
        memset(buf, 0, sizeof(buf));
    }

    ifs.close();
    return true;
}