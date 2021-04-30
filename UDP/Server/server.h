#ifndef _SERVER_H
#define _SERVER_H

#include <winsock2.h>

#include <iostream>
#include <fstream>
#include <chrono>

#pragma comment(lib, "ws2_32.lib")

#define BUFSIZE 1024

class Server {
 private:
  // socket版本
  WORD wVersionRequested;
  WSADATA wsaData;

  // 描述服务器的套接字
  SOCKET sockfd;

  // 描述服务器信息的结构体
  struct sockaddr_in server;

  // 描述客户端的结构体
  struct sockaddr_in udpAddr;

  // 缓冲数组
  char buf[BUFSIZE];

  // 接受反馈
  int rval = 0;

  // 文件流指针，用于打开文件
  std::ofstream ofs;

 public:
  // 存放文件名
  char file[BUFSIZE];

  // 存放客户端IP
  char clientIp[128];

  // 客户端端口
  int clientPort;

 public:
  // 描述文件大小
  size_t fileSize;
  // 描述已发送的字节
  size_t recvSize;
  // 接受文件所用时间
  double time;

 public:
  Server(const int port);
  ~Server();

  // TCP连接建立以及相关信息传输
  bool work();

 private:
  // 通过套接字接受文件
  bool recvFile(const char filename[], double &time);

};

#endif
