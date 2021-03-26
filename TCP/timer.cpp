#include "timer.h"

void work(bool& isRunning, int timeout)
{
    std::unique_lock<std::mutex> sleepLock(sleepMutex);
    while(isRunning)
    {
        condition.wait_for(sleepLock, std::chrono::milliseconds(timeout));

        auto timePoint = std::chrono::system_clock::now();
        std::time_t currentTime = std::chrono::system_clock::to_time_t(timePoint);

        std::cout << "currentTime: " << currentTime << " isRunning: " << isRunning << std::endl;
    }
}

Timer::Timer(double timeoutMillis)
{
    this->timeout = timeoutMillis;
    this->isRunning = false;
}

Timer::~Timer()
{
    if(isRunning)
    {
        isRunning = false;
    }
}

void Timer::start()
{
    isRunning = true;
    std::thread th = std::thread(work, std::ref(isRunning), timeout);
    th.detach();
}

void Timer::stop()
{
    isRunning = false;
}
