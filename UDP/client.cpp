#include "client.h"


Client::Client(const char addr[], const int port)
{
    fileSize = 0;
    sentBytes = 0;

    wVersionRequested = MAKEWORD(2, 2);
    WSAStartup(wVersionRequested, &wsaData);
    if ((c_socket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
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
    rval = connect(c_socket, (sockaddr*)&serverAddr, sizeof(serverAddr));
    if (rval == -1) 
    {
        std::cout << "Can not create connect!" << std::endl;
        exit(0);
    }

    name = path;
    size_t lastPos = name.find_last_of("/\\");
    name = name.substr(lastPos + 1);
	char* filename = const_cast<char*>(name.c_str());

    strcpy(buf, filename);
    send(c_socket, buf, BUFSIZE, 0);

    recv(c_socket, buf, BUFSIZE, 0);
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

    ifs.open(path, std::ios::in | std::ios::binary);
    if(!ifs.is_open())
    {
        std::cout << "Can not open " << path << std::endl;
        exit(0);
    }

    ifs.seekg(0, std::ios::end);
    fileSize = ifs.tellg();
    ifs.seekg(0, std::ios::beg);

    memset(buf, 0, sizeof(buf));
    while(!ifs.eof())
    {
        ifs.read(buf, BUFSIZE);
        readLen = ifs.gcount();
        sentBytes += readLen;
        if(send(c_socket, buf, readLen, 0) == -1)
        {
            ifs.close();
            return false;
        }
        memset(buf, 0, sizeof(buf));
    }

    ifs.close();
    return true;
}