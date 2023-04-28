//
// Created by huste on 2023-04-21.
//

#include "IDecode.h"
#include "Log.h"

void IDecode::Main()
{
    while(!isExit)
    {
        packetMutex.lock();

        //synchronize audio video
        if(!isAudio && synPts > 0)
        {
            if(synPts < pts)
            {
                packetMutex.unlock();
                Sleep(1);
                continue;
            }
        }

        if(packets.empty())
        {
            packetMutex.unlock();
            Sleep(1);
            continue;
        }
        //extract packets, consumer
        Data pack = packets.front();
        packets.pop_front();

        //send data to decoder thread, one packet could have multiple results
        if(this->SendPacket(pack))
        {
            while(!isExit)
            {
                Data frame = RecieveFrame();
                if(!frame.data) break;
                //LOGE("Recieve frame %d", frame.size);
                pts = frame.pts;
                this->Notify(frame);
            }
        }
        pack.Drop();
        packetMutex.unlock();
    }

}

void IDecode::Update(Data pkt)
{
    if(pkt.isAudio != isAudio)
    {
        return;
    }
    while(!isExit)
    {
        packetMutex.lock();

        if(packets.size() < maxList)
        {
            //producer
            packets.push_back(pkt);
            packetMutex.unlock();
            break;
        }
        packetMutex.unlock();
        Sleep(1);
    }
}

void IDecode::Clear()
{

    packetMutex.lock();
    while(!packets.empty())
    {
        packets.front().Drop();
        packets.pop_front();

    }
    pts = 0;
    synPts = 0;
    packetMutex.unlock();
}
