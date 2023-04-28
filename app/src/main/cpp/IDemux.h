//
// Created by huste on 2023-04-19.
//

#ifndef MEDIAPLAYER_IDEMUX_H
#define MEDIAPLAYER_IDEMUX_H

#include "Data.h"
#include "Thread.h"
#include "IObserver.h"
#include "Parameter.h"

//
class IDemux : public IObserver
{
public:
    //opening file
    virtual bool Open(const char* url) = 0;
    //reads one frame, required manual clean up
    virtual Data Read() = 0;

    virtual Parameter GetVideoPara() = 0;
    virtual Parameter GetAudioPara() = 0;
    virtual void Close() = 0;
    //video duration
    int totalTime = 0;
protected:
    virtual void Main();
};


#endif //MEDIAPLAYER_IDEMUX_H
