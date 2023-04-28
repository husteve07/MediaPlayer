//
// Created by huste on 2023-04-22.
//

#ifndef MEDIAPLAYER_IAUDIOPLAY_H
#define MEDIAPLAYER_IAUDIOPLAY_H


#include <list>
#include "IObserver.h"
#include "Parameter.h"

class IAudioPlay : public IObserver
{
public:
    virtual void Update(Data data);
    virtual Data GetData();
    virtual bool StartPlay(Parameter out) = 0;
    virtual void Close() = 0;
    virtual void Clear() ;

    int maxFrame = 100;
    int pts = 0;
protected:
    std::list<Data> frames;
    std::mutex framesMutex;
};


#endif //MEDIAPLAYER_IAUDIOPLAY_H
