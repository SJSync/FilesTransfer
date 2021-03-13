#include <iostream>
#include <iomanip>
#include "server.h"

using namespace std;

int main(int argc, char **argv)
{
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
}

// g++ main.cpp server.cpp -lws2_32 -o server