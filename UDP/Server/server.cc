#include "server.h"

// 初始化socket
Server::Server(const int port) {
  // socket版本
  wVersionRequested = MAKEWORD(2, 2);
  WSAStartup(wVersionRequested, &wsaData);

  // 建立套接字
  if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET) {
    //异常处理
    std::cout << "Can not create socket!" << std::endl;
    exit(0);
  }

  // 设置地址簇为Internet协议族
  server.sin_family = AF_INET;
  // 设置端口
  server.sin_port = htons(port);
  // server.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
  // 设置IP地址任何地址
  server.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
  // 填充0
  memset(server.sin_zero, 0, sizeof(server.sin_zero));

  // 将本机IP和端口与sockfd绑定
  rval = bind(sockfd, (struct sockaddr *) &server, sizeof(server));
  if (rval == -1) {
    // 异常处理
    std::cout << "Can not create connect!" << std::endl;
    exit(0);
  }

}

// 析构函数，如果套接字存在，就关闭
Server::~Server() {
  if (sockfd != INVALID_SOCKET) {
    closesocket(sockfd);
  }
  // 清除Socket信息
  WSACleanup();
}

bool Server::work() {
  char filename[BUFSIZE] = {0};
  int udpAddrLenth = sizeof(udpAddr);

  // 接收文件名
  recvfrom(sockfd, filename, BUFSIZE, 0, (struct sockaddr *) &udpAddr, &udpAddrLenth);
  strcpy(file, filename);

  // 转换IP地址
  strcpy(clientIp, inet_ntoa(udpAddr.sin_addr));
  clientPort = ntohs(udpAddr.sin_port);

  // 发送确认信息
  strcpy(buf, "recv");
  sendto(sockfd, buf, BUFSIZE, 0, (struct sockaddr *) &udpAddr, sizeof(udpAddr));

  // 接受文件
  if (!recvFile(filename, time)) {
    return false;
  }
  // 发送接受完成的信息
  strcpy(buf, "recvDone");
  sendto(sockfd, buf, BUFSIZE, 0, (struct sockaddr *) &udpAddr, sizeof(udpAddr));
  // 接收成功后就关闭套接字
  closesocket(sockfd);

  return true;
}

bool Server::recvFile(const char filename[], double &time) {
  // 获取当前系统时间
  auto start = std::chrono::system_clock::now();

  recvSize = 0;
  // readLen标识从文件流中读取到buf缓冲数组中的字节数
  int readLen = 0;

  // 创建接受的文件流
  ofs.open(filename, std::ios::out | std::ios::binary);
  if (!ofs.is_open()) {
    std::cout << "Can not open " << filename << std::endl;
    exit(0);
  }

  // 初始化buf缓冲数组为0
  memset(buf, 0, sizeof(buf));
  do {
    int udpAddrLenth = sizeof(udpAddr);
    readLen = recvfrom(sockfd, buf, BUFSIZE, 0, (struct sockaddr *) &udpAddr, &udpAddrLenth);
    if (readLen == -1) {
      // 异常处理
      ofs.close();
      return false;
    }
    recvSize += readLen;

    // 将buf缓冲数组的内容写入到文件流
    ofs.write(buf, readLen);

    if (readLen < BUFSIZE) {
      // readLen小于缓冲区大小说明远端已经传输完成
      break;
    }

    // 初始化buf缓冲数组为0
    memset(buf, 0, sizeof(buf));
  } while (true);

  // 接收成功后就关闭文件流
  ofs.close();

  fileSize = recvSize;

  // 获取当前系统时间，与开始时间相减得到接收文件所用时间
  std::chrono::duration<double> diff = std::chrono::system_clock::now() - start;
  time = diff.count();

  return true;
}