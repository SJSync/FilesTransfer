#include "server.h"

using namespace std;

Socket::Socket(const int port)
{
    wVersionRequested = MAKEWORD(2, 2);
    WSAStartup(wVersionRequested, &wsaData);
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
    {
        cout << "Can not create socket!" << endl;
        exit(0);
    }

    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    // server.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
    server.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
    memset(server.sin_zero, 0, sizeof(server.sin_zero));

    rval = bind(sockfd, (struct sockaddr*)&server, sizeof(server));
    if (rval == -1) 
    {
        cout << "Can not create connect!" << endl;
        exit(0);
    }

    listen(sockfd, 5);
}

Socket::~Socket()
{
    if(s_socket != INVALID_SOCKET)
    {
        closesocket(s_socket);
    }
    closesocket(sockfd);
    WSACleanup();
}

bool Socket::work()
{
    char filename[BUFSIZE] = {0};
    int tcpAddrLenth = sizeof(tcpAddr);
    if((s_socket = accept(sockfd, (struct sockaddr*)&tcpAddr, &tcpAddrLenth)) == INVALID_SOCKET)
    {
        cout << "Accept exception" << endl;
        exit(0);
    }
    
    strcpy(clientIp, inet_ntoa(tcpAddr.sin_addr));
    clientPort = ntohs(tcpAddr.sin_port);

    recv(s_socket, filename, BUFSIZE, 0);
    strcpy(file, filename);

    strcpy(buf, "recv");
    send(s_socket, buf, BUFSIZE, 0);

    if(!recvFile(filename))
    {
        return false;
    }
    closesocket(s_socket);

    return true;
}

bool Socket::recvFile(const char filename[])
{
    int readLen = 0;
    ofs.open(filename, ios::out | ios::binary);
    if(!ofs.is_open())
    {
        cout << "Can not open the file" << filename << endl;
        exit(0);
    }

    memset(buf, 0, sizeof(buf));
    do
    {
        readLen = recv(s_socket, buf, BUFSIZE, 0);
        if(readLen == -1)
	    {
	        ofs.close();
            return false;
	    }
        else if(readLen == 0)
        {
            break;
        }
	    ofs.write(buf, readLen);
        memset(buf, 0, sizeof(buf));
    } while(true);

    ofs.close();
    return true;
}