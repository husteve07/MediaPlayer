//
// Created by huste on 2023-04-26.
//

#ifndef MEDIAPLAYER_IPLAYERPROXY_H
#define MEDIAPLAYER_IPLAYERPROXY_H


#include "IPlayer.h"
#include <mutex>
class IPlayerProxy : public IPlayer
{
public:
    static IPlayerProxy* Get()
    {
        static IPlayerProxy px;
        return &px;
    }
    void Init(void* vm = 0);
    virtual bool Open(const char* path);
    virtual void Close();
    virtual bool Start();
    virtual void InitView(void* win);

protected:
    IPlayerProxy(){}
    IPlayer *player = 0;
    std::mutex mux;

};


#endif //MEDIAPLAYER_IPLAYERPROXY_H
