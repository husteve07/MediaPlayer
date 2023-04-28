//
// Created by huste on 2023-04-19.
//

#include "Thread.h"
#include <thread>
#include "Log.h"
bool Thread::Start()
{
    isExit = false;
    std::thread th(&Thread::ThreadMain, this);
    th.detach();
    return true;
}
void Sleep(int ms)
{
    std::chrono::milliseconds  du(ms);
    std::this_thread::sleep_for(du);
}
void Thread::Stop()
{
    isExit = true;
    for(int i = 0; i < 200; i++)
    {
        if(!isRunning)
        {
            LOGI("Stop thread ok");
            return;
        }
        Sleep(1);
    }
    LOGE("Thread stop Time out");
}

void Thread::ThreadMain()
{
    isRunning = true;
    LOGI("Using Thread function");
    Main();
    LOGI("Exiting Thread function");
    isRunning = false;
}


