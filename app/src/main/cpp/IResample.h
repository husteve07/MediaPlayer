//
// Created by huste on 2023-04-22.
//

#ifndef MEDIAPLAYER_IRESAMPLE_H
#define MEDIAPLAYER_IRESAMPLE_H


#include "IObserver.h"
#include "Parameter.h"

class IResample : public IObserver
{
public:
    virtual bool Open(Parameter in, Parameter out = Parameter()) = 0;
    virtual Data Resample(Data indata) = 0;
    virtual void Close() = 0;
    virtual void Update(Data data);
    int outChannels = 2;
    int outFormat = 1;
};


#endif //MEDIAPLAYER_IRESAMPLE_H
