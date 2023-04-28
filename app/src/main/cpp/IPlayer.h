//
// Created by huste on 2023-04-25.
//

#ifndef MEDIAPLAYER_IPLAYER_H
#define MEDIAPLAYER_IPLAYER_H

#include <mutex>
#include "Thread.h"
#include "Parameter.h"

class IDemux;
class IDecode;
class IResample;
class IVideoView;
class IAudioPlay;


class IPlayer : public Thread
{
public :
    static IPlayer* Get(unsigned char  index = 0);
    virtual bool Open(const char* path);
    virtual void Close();
    virtual bool Start();
    virtual void InitView(void* win);


    bool isHardDecode = true;
    Parameter outpara;

    IDemux* demux = 0;
    IDecode* vdecode = 0;
    IDecode* adecode = 0;
    IResample* resample = 0;
    IVideoView* videoview = 0;
    IAudioPlay* audioplay = 0;
protected:
    void Main();
    std::mutex mux;
    IPlayer() {};
};


#endif //MEDIAPLAYER_IPLAYER_H
