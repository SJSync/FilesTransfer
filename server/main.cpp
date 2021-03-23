#include <iostream>
#include <iomanip>
#include "server.h"

using namespace std;

void help()
{
    cout << "server.exe [Listening Port]" << endl;
    cout << endl;
}

int main(int argc, char **argv)
{
    if(argc == 1 || (argc == 2 &&(strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0)))
    {
        help();
        return 0;
    }

    int port = atoi(argv[1]);
    bool flag = false;

    Socket s_socket(port);
    while(true)
    {
        flag = s_socket.work();
        if(flag)
        {
            cout << left << setw(18) << s_socket.clientIp 
            << left << setw(10) << "Successd:" << s_socket.file << endl;
        }
        else
        {
            cout << left << setw(18) << s_socket.clientIp 
            << left << setw(10) << "Failed:" << s_socket.file << endl;
        }
    }
    return 0;
}

// g++ --static main.cpp server.cpp -l ws2_32 -o server