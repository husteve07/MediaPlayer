//
// Created by huste on 2023-04-22.
//

#include "IAudioPlay.h"
#include "Log.h"

void IAudioPlay::Update(Data data)
{
    //LOGE("IAudio::update used %d ", data.size);
    if(data.size <=0 || !data.data ) return;
    while(!isExit)
    {
        framesMutex.lock();
        if(frames.size() > maxFrame)
        {
            framesMutex.unlock();
            Sleep(1);
            continue;
        }
        frames.push_back(data);
        framesMutex.unlock();
        break;
    }

}

Data IAudioPlay::GetData()
{
    Data d;
    while(!isExit)
    {
        framesMutex.lock();
        if(!frames.empty())
        {
            d = frames.front();
            frames.pop_front();
            framesMutex.unlock();
            pts = d.pts;
            return d;
        }
        framesMutex.unlock();
        Sleep(1);
    }
    return d;
}

void IAudioPlay::Clear()
{
    framesMutex.lock();

    while(!frames.empty())
    {
        frames.front().Drop();
        frames.pop_front();
    }

    framesMutex.unlock();
}
