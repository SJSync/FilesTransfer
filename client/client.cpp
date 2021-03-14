#include "client.h"

using namespace std;

Socket::Socket(const char addr[], const int port)
{
    wVersionRequested = MAKEWORD(2, 2);
    WSAStartup(wVersionRequested, &wsaData);
    if ((c_socket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
    {
        cout << "Can not create socket!" << endl;
        exit(0);
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.S_un.S_addr = inet_addr(addr);
    memset(serverAddr.sin_zero, 0, sizeof(serverAddr.sin_zero));
}

Socket::~Socket()
{
    if(c_socket != INVALID_SOCKET)
    {
        closesocket(c_socket);
    }
    WSACleanup();
}

bool Socket::work(const char path[])
{
    rval = connect(c_socket, (sockaddr*)&serverAddr, sizeof(serverAddr));
    if (rval == -1) 
    {
        cout << "Can not create connect!" << endl;
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
        if(!sendFile(filename))
        {
            return false;
        }
        closesocket(c_socket);
    }
    return true;
}

bool Socket::sendFile(const char filename[])
{
    int readLen = 0;
    ifs.open(filename, ios::in | ios::binary);
    if(!ifs.is_open()) 
    {
        cout << "Can not open the file" << filename << endl;
        exit(0);
    }

    memset(buf, 0, sizeof(buf));
    while(!ifs.eof())
    {
        ifs.read(buf, BUFSIZE);
        readLen = ifs.gcount();
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