//
// Created by huste on 2023-04-19.
//

#ifndef MEDIAPLAYER_FFMPEGDEMUX_H
#define MEDIAPLAYER_FFMPEGDEMUX_H

#include "IDemux.h"
#include <mutex>
struct AVFormatContext;

class FFmpegDemux : public IDemux
{
public:
    FFmpegDemux();
    //opening file
    virtual bool Open(const char* url);
    //reads one frame, required manual clean up
    virtual void Close();
    virtual Data Read();

    virtual Parameter GetVideoPara() ;
    virtual Parameter GetAudioPara() ;

private:
    AVFormatContext* ic = 0; //only works for no args constructor
    std::mutex mux;
    int audioStream = 1;
    int videoStream = 0;

};


#endif //MEDIAPLAYER_FFMPEGDEMUX_H
