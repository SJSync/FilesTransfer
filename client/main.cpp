#include <iostream>
#include <iomanip>
#include "client.h"

using namespace std;

int main(int argc, char **argv)
{
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
            cout << left << setw(10) << "Successd:" << argv[i] << endl;
        }
        else
        {
            //cout << "ip: " << argv[1] << ":" << port << endl;
            cout << left << setw(10) << "Failed:" << argv[i] << endl;
        }
        delete c_socket;
        c_socket = NULL;
    }
    return 0;
}

// g++ main.cpp client.cpp -lws2_32 -o client