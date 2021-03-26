#include "server.h"

Socket::Socket(const int port)
{
    wVersionRequested = MAKEWORD(2, 2);
    WSAStartup(wVersionRequested, &wsaData);
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
    {
        std::cout << "Can not create socket!" << std::endl;
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
        std::cout << "Can not create connect!" << std::endl;
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
        std::cout << "Accept exception" << std::endl;
        exit(0);
    }
    
    strcpy(clientIp, inet_ntoa(tcpAddr.sin_addr));
    clientPort = ntohs(tcpAddr.sin_port);

    recv(s_socket, filename, BUFSIZE, 0);
    strcpy(file, filename);

    strcpy(buf, "recv");
    send(s_socket, buf, BUFSIZE, 0);

    if(!recvFile(filename, time))
    {
        return false;
    }
    closesocket(s_socket);

    return true;
}

bool Socket::recvFile(const char filename[], double& time)
{
    auto start = std::chrono::system_clock::now();

    fileSize = 0;
    int readLen = 0;
    ofs.open(filename, std::ios::out | std::ios::binary);
    if(!ofs.is_open())
    {
        std::cout << "Can not open " << filename << std::endl;
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
        fileSize += readLen;
	    ofs.write(buf, readLen);
        memset(buf, 0, sizeof(buf));
    } while(true);

    ofs.close();

    std::chrono::duration<double> diff = std::chrono::system_clock::now() - start;
    time = diff.count();
    return true;
}