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

void help(const char* progname)
{
    cout << progname << " [Listening Port]" << endl;
    cout << endl;
}

int main(int argc, char **argv)
{
    if(argc == 1 || (argc == 2 &&(strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0)))
    {
        help(argv[0]);
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
            cout << left << setw(18) << s_socket.clientIp << left << setw(10) << "Successd:" 
            << s_socket.file << " [" << s_socket.fileSize * 8 / 1024 / 1024 / s_socket.time << "Mbps]" << endl;
        }
        else
        {
            cout << left << setw(18) << s_socket.clientIp 
            << left << setw(10) << "Failed:" << s_socket.file << endl;
        }
    }
    return 0;
}

// g++ --static ServerMain.cpp server.cpp -l ws2_32 -o server