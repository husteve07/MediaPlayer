//
// Created by huste on 2023-04-19.
//

#ifndef MEDIAPLAYER_THREAD_H
#define MEDIAPLAYER_THREAD_H

void Sleep(int ms);

class Thread
{
public:
    virtual bool Start();
    virtual void Stop();
    virtual void Main(){}
protected:
    bool isExit = false;
    bool isRunning = false;
private:
    void ThreadMain();

};


#endif //MEDIAPLAYER_THREAD_H
