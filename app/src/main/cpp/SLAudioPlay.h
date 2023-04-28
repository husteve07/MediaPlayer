//
// Created by huste on 2023-04-22.
//

#ifndef MEDIAPLAYER_SLAUDIOPLAY_H
#define MEDIAPLAYER_SLAUDIOPLAY_H


#include "IAudioPlay.h"

class SLAudioPlay : public IAudioPlay
{
public:
    virtual bool StartPlay(Parameter out);
    void PlayCall(void* bufq);
    virtual void Close();
    SLAudioPlay();
    virtual ~SLAudioPlay();
protected:
    std::mutex mux;
    unsigned char* buf = 0;
};


#endif //MEDIAPLAYER_SLAUDIOPLAY_H
