//
// Created by huste on 2023-04-21.
//

#ifndef MEDIAPLAYER_FFMPEGDECODE_H
#define MEDIAPLAYER_FFMPEGDECODE_H

#include "Parameter.h"
#include "IDecode.h"

struct AVCodecContext;
struct AVFrame;
class FFmpegDecode : public IDecode
{
public:
    static void InitHard(void *vm);

    virtual bool Open(Parameter para, bool isHard = false);

    virtual void Close();

    //future model
    virtual bool SendPacket(Data pkt);

    //get result from thread
    virtual Data RecieveFrame();

protected:
    AVCodecContext* codec = 0;
    AVFrame* frame = 0;
    std::mutex mux;
};




#endif //MEDIAPLAYER_FFMPEGDECODE_H
