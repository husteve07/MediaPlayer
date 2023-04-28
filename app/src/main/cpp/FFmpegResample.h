//
// Created by huste on 2023-04-22.
//

#ifndef MEDIAPLAYER_FFMPEGRESAMPLE_H
#define MEDIAPLAYER_FFMPEGRESAMPLE_H

#include "IResample.h"
struct SwrContext;

class FFmpegResample : public IResample
{
public:
    virtual bool Open(Parameter in, Parameter out = Parameter());
    virtual void Close();
    virtual Data Resample(Data indata);
protected:
    SwrContext* actx = 0;
    std::mutex mux;

};


#endif //MEDIAPLAYER_FFMPEGRESAMPLE_H
