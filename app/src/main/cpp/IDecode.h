//
// Created by huste on 2023-04-21.
//

#ifndef MEDIAPLAYER_IDECODE_H
#define MEDIAPLAYER_IDECODE_H
#include "Parameter.h"
#include "IObserver.h"
#include <list>
//supports hardware decoding
class IDecode : public IObserver
{
public:

    virtual bool Open(Parameter para, bool isHard = false) = 0;

    //future model
    virtual bool SendPacket(Data pkt) = 0;

    //get result from thread
    virtual Data RecieveFrame() = 0;

    //on notify
    virtual void Update(Data pkt);

    virtual void Close() = 0;

    virtual void Clear();

    bool isAudio = false;

    //largest queue buffer
    int maxList = 100;

    //synch time
    int synPts = 0;
    int pts = 0;
protected:
    virtual void Main();
    //buffer
    std::list<Data> packets;
    std::mutex packetMutex;
};


#endif //MEDIAPLAYER_IDECODE_H
