#include <iostream>
#include <condition_variable>
#include <chrono>
#include <thread>
#include <mutex>

std::mutex sleepMutex;
std::condition_variable condition;

class Timer
{
private:
    int timeout;
    bool isRunning;

public:;
    void start();   
    void stop();

public:
    Timer(double timeout);
    ~Timer();
};
