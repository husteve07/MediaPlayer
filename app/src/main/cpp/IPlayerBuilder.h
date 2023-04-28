//
// Created by huste on 2023-04-26.
//

#ifndef MEDIAPLAYER_IPLAYERBUILDER_H
#define MEDIAPLAYER_IPLAYERBUILDER_H
#include "IPlayer.h"
#include "IDecode.h"
#include "IDemux.h"
#include "IResample.h"
#include "IVideoView.h"
#include "IAudioPlay.h"
class IPlayerBuilder
{
public:
    virtual IPlayer* BuildPlayer(unsigned const char index = 0);
protected:
    virtual IDemux* CreateDemux() = 0;
    virtual IDecode* CreateDecode() = 0;
    virtual IResample* CreateResample() = 0;
    virtual IVideoView* CreateVideoview() = 0;
    virtual IAudioPlay* CreateAudioPlay() = 0;
    virtual IPlayer* CreatePlayer(unsigned const char index) = 0;
};


#endif //MEDIAPLAYER_IPLAYERBUILDER_H
