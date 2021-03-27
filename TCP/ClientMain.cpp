#include <iostream>
#include <iomanip>
#include "client.h"
#include "timer.h"

using namespace std;

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

    char* serverip = argv[1];
    int port = atoi(argv[2]);
    bool flag = false;

    Client* client = NULL;

    cout << endl;
    for(int i = 3; i < argc; i++)
    {
        client = new Client(serverip, port);
        Timer timer(200);
        
        timer.start(client->fileSize, client->sentBytes);
        flag = client->work(argv[i]);
        timer.stop();

        if(flag)
        {
            cout << left << setw(10) << "Successd: " << left << setw(20) << client->name;
            cout << " Size: " + std::to_string(client->fileSize / 1024 / 1024) + "MB" << endl;
        }
        else
        {
            cout << left << setw(10) << "Failed:" << client->name << endl;
        }

        delete client;
        client = NULL;
    }
    cout << endl;
    return 0;
}

// g++ --static ClientMain.cpp client.cpp -l ws2_32 -o client