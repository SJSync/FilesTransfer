#include <iostream>
#include <iomanip>
#include "client.h"

using namespace std;

void help(const char* progname)
{
    cout << progname << " [IP Address] [Port] [FilePath_1] [FilePath_2] ..." << endl;
    cout << endl;
}

int main(int argc, char **argv)
{
    if(argc == 1 || (argc == 2 &&(strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0)))
    {
        help(argv[0]);
        return 0;
    }

    char* serverip = argv[1];
    int port = atoi(argv[2]);
    bool flag = false;

    Socket* c_socket = NULL;

    for(int i = 3; i < argc; i++)
    {
        c_socket = new Socket(serverip, port);
        flag = c_socket->work(argv[i]);
        if(flag)
        {
            cout << "Size: " << c_socket->fileSize / 1024 / 1024 << "MB" << endl;
            cout << left << setw(10) << "Successd:" << c_socket->name << endl;
        }
        else
        {
            cout << left << setw(10) << "Failed:" << c_socket->name << endl;
        }
        delete c_socket;
        c_socket = NULL;
    }
    cout << endl;
    return 0;
}

// g++ --static main.cpp client.cpp -l ws2_32 -o client