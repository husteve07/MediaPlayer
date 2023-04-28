//
// Created by huste on 2023-04-26.
//

#include "FFmpegPlayerBuilder.h"
#include "FFmpegDemux.h"
#include "FFmpegDecode.h"
#include "FFmpegResample.h"
#include "GLVideoView.h"
#include "SLAudioPlay.h"


IDemux *FFmpegPlayerBuilder::CreateDemux()
{
    IDemux* ff = new FFmpegDemux();
    return ff;
}


IDecode *FFmpegPlayerBuilder::CreateDecode()
{
    IDecode* ff = new FFmpegDecode();
    return ff;
}


IResample *FFmpegPlayerBuilder::CreateResample()
{
    IResample* ff = new FFmpegResample();
    return ff;
}


IVideoView *FFmpegPlayerBuilder::CreateVideoview()
{
    IVideoView* ff = new GLVideoView();
    return ff;
}


IAudioPlay *FFmpegPlayerBuilder::CreateAudioPlay()
{
    IAudioPlay* ff = new SLAudioPlay();
    return ff;
}


IPlayer *FFmpegPlayerBuilder::CreatePlayer(const unsigned char index)
{
    return IPlayer::Get(index);
}

void FFmpegPlayerBuilder::InitHard(void *vm)
{
    FFmpegDecode::InitHard(vm);
}
