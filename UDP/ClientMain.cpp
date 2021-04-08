#include <iostream>
#include <iomanip>
#include "client.h"
#include "timer.h"

using namespace std;

// 帮助函数，传入argv第一个字符指针
void help(const char* name)
{
    // 裁切掉文件路径，保留文件名
    std::string progname = name;
    size_t lastPos = progname.find_last_of("/\\");
    progname = progname.substr(lastPos + 1);

    cout << ".\\" << progname << " [IP Address] [Port] [FilePath_1] [FilePath_2] ..." << endl;
    cout << endl;
}

int main(int argc, char **argv)
{
    if(argc == 1 || (argc == 2 &&(strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0)))
    {
        help(argv[0]);
        return 0;
    }

    // serverIP地址
    char* serverip = argv[1];
    // server端口号
    int port = atoi(argv[2]);
    bool flag = false;

    // 新建一个client指针
    Client* client = NULL;

    cout << endl;
    for(int i = 3; i < argc; i++)
    {
        // 新建一个Client对象
        client = new Client(serverip, port);
        // 初始化计时器，每隔两百毫秒就触发一次进度显示函数
        Timer timer(200);
        
        // 开始计时器
        timer.start(client->fileSize, client->sentBytes);
        // 开始文件传输
        flag = client->work(argv[i]);
        // 结束计时器
        timer.stop();

        // 如果文件传输成功，打印对应信息
        if(flag)
        {
            cout << left << setw(10) << "Successd: " << left << setw(20) << client->name;
            cout << " Size: " + std::to_string(client->fileSize / 1024 / 1024) + "MB " << endl;
        }
        // 如果文件传输失败，打印对应信息
        else
        {
            cout << left << setw(10) << "Failed:" << client->name << endl;
        }

        // 删除client对象
        delete client;
        client = NULL;
        Sleep(100);
    }
    cout << endl;
    return 0;
}

// g++ --static ClientMain.cpp client.cpp timer.cpp -l ws2_32 -o client
