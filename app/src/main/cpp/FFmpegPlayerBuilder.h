//
// Created by huste on 2023-04-26.
//

#ifndef MEDIAPLAYER_FFMPEGPLAYERBUILDER_H
#define MEDIAPLAYER_FFMPEGPLAYERBUILDER_H


#include "IPlayerBuilder.h"

class FFmpegPlayerBuilder : public IPlayerBuilder {
public:
    static void InitHard(void* vm);
    static FFmpegPlayerBuilder *Get()
    {
        static FFmpegPlayerBuilder ff;
        return &ff;
    }

protected:
    FFmpegPlayerBuilder(){};
    virtual IDemux* CreateDemux() ;
    virtual IDecode* CreateDecode() ;
    virtual IResample* CreateResample() ;
    virtual IVideoView* CreateVideoview() ;
    virtual IAudioPlay* CreateAudioPlay() ;
    virtual IPlayer* CreatePlayer(unsigned const char index) ;
};


#endif //MEDIAPLAYER_FFMPEGPLAYERBUILDER_H
