#include "client.h"


// 构造函数，初始化socket
Client::Client(const char addr[], const int port)
{
    // 初始化文件大小和已发送字节
    fileSize = 0;
    sentBytes = 0;

    // 初始化socket版本
    wVersionRequested = MAKEWORD(2, 2);
    WSAStartup(wVersionRequested, &wsaData);

    // 建立描述服务器的套接字
    if ((s_socket = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET)
    {
        std::cout << "Can not create socket!" << std::endl;
        exit(0);
    }

    // 设置地址簇为Internet协议族
    serverAddr.sin_family = AF_INET;
    // 设置端口
    serverAddr.sin_port = htons(port);
    // 设置IP地址
    serverAddr.sin_addr.S_un.S_addr = inet_addr(addr);
    // 填充0
    memset(serverAddr.sin_zero, 0, sizeof(serverAddr.sin_zero));
}

// 析构函数，如果套接字存在，就关闭
Client::~Client()
{
    if(s_socket != INVALID_SOCKET)
    {
        closesocket(s_socket);
    }
    // 清除Socket信息
    WSACleanup();
}

bool Client::work(const char path[])
{
    int addr_len = sizeof(serverAddr);

    // 从文件路径截取文件名
    name = path;
    size_t lastPos = name.find_last_of("/\\");
    name = name.substr(lastPos + 1);
	char* filename = const_cast<char*>(name.c_str());

    // 向远端发送文件名
    strcpy(buf, filename);
    sendto(s_socket, buf, BUFSIZE, 0, (struct sockaddr*)&serverAddr, sizeof(serverAddr));

    // 确认收到文件名
    recvfrom(s_socket, buf, BUFSIZE, 0, (struct sockaddr*)&serverAddr, &addr_len);
    if(strcmp(buf, "recv") == 0)
    {
        // 开始发送文件
        if(!sendFile(path))
        {
            // 发送错误就返回
            return false;
        }
        // 发送成功后就关闭套接字
        closesocket(s_socket);
    }
    return true;
}

bool Client::sendFile(const char path[])
{
    // readLen标识从文件流中读取到buf缓冲数组中的字节数
    int readLen = 0;
    sentBytes = 0;

    //打开发送的文件
    ifs.open(path, std::ios::in | std::ios::binary);
    // 打开异常处理
    if(!ifs.is_open())
    {
        std::cout << "Can not open " << path << std::endl;
        exit(0);
    }

    // 移动文件指针到文件流末尾，获取文件的字节数
    ifs.seekg(0, std::ios::end);
    fileSize = ifs.tellg();
    ifs.seekg(0, std::ios::beg);

    // 初始化buf缓冲数组为0
    memset(buf, 0, sizeof(buf));
    // 读取文件流，如果文件流抛出eof说明已经读完
    while(!ifs.eof())
    {
        // 从文件流读取到buf缓冲数组
        ifs.read(buf, BUFSIZE);
        // 返回读取到buf缓冲数组的字节数
        readLen = ifs.gcount();
        sentBytes += readLen;
        // 从buf缓冲数组发送readLen长度到远端
        if(sendto(s_socket, buf, readLen, 0, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1)
        {
            // 发送异常处理
            ifs.close();
            return false;
        }

        // std::cout << readLen << std::endl;

        // 初始化buf缓冲数组为0
        memset(buf, 0, sizeof(buf));
    }

    // 发送成功就关闭文件流
    ifs.close();
    return true;
}