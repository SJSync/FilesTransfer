#include <iostream>
#include <iomanip>
#include <chrono>

#include "server.h"

using namespace std;

/*
template <class T>
void measure(T&& func)
{
    auto start = chrono::system_clock::now();
    func();
    chrono::duration<double> diff = chrono::system_clock::now() - start;
    cout << diff << endl;
}
*/

// 帮助函数，传入argv第一个字符指针
void help(const char *name) {
  // 裁切掉文件路径，保留文件名
  std::string progname = name;
  size_t lastPos = progname.find_last_of("/\\");
  progname = progname.substr(lastPos + 1);

  cout << ".\\" << progname << " [Port]" << endl;
  cout << endl;
}

int main(int argc, char **argv) {
  if (argc == 1 || (argc == 2 && (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0))) {
    help(argv[0]);
    return 0;
  }

  // 绑定的端口号
  int port = atoi(argv[1]);
  bool flag = false;

  // 新建一个Server对象
  Server server(port);
  while (true) {
    // 开始文件接收
    flag = server.work();
    // 如果文件接收成功，打印对应信息
    if (flag) {
      cout << left << setw(18) << server.clientIp;
      cout << left << setw(10) << "Successd:" << left << setw(20) << server.file;
      cout << " Size: " << left << setw(10) << std::to_string(server.fileSize / 1024 / 1024) + "MB";
      cout << "  [" + std::to_string(server.fileSize * 8 / 1024 / 1024 / server.time) + "Mbps] " << endl;
    }
      // 如果文件接收失败，打印对应信息
    else {
      cout << left << setw(18) << server.clientIp
           << left << setw(10) << "Failed:" << server.file << endl;
    }
  }
  return 0;
}

// g++ --static ServerMain.cpp server.cpp -l ws2_32 -o server