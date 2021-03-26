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

void help(const char* name)
{
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

    int port = atoi(argv[1]);
    bool flag = false;

    Server server(port);
    while(true)
    {
        flag = server.work();
        if(flag)
        {
            cout << left << setw(18) << server.clientIp ;
            cout << left << setw(10) << "Successd:" << server.file;
            cout << right << setw(10) << " Size:" << server.fileSize / 1024 / 1024 << "MB";
            cout << right << setw(5) << " [" << server.fileSize * 8 / 1024 / 1024 / server.time << "Mbps]" << endl;
        }
        else
        {
            cout << left << setw(18) << server.clientIp 
            << left << setw(10) << "Failed:" << server.file << endl;
        }
    }
    return 0;
}

// g++ --static ServerMain.cpp server.cpp -l ws2_32 -o server