//
// Created by huste on 2023-04-26.
//

#include "IPlayerBuilder.h"

IPlayer *IPlayerBuilder::BuildPlayer(const unsigned char index)
{
    IPlayer* player = CreatePlayer(index);

    //unpack
    IDemux* de = CreateDemux();

    //decode
    IDecode* vdecode = CreateDecode();
    IDecode* adecode = CreateDecode();

    //add observer
    de->AddObs(vdecode);
    de->AddObs(adecode);

    //display
    IVideoView* view = CreateVideoview();
    vdecode->AddObs(view);

    //resampling
    IResample* resample = CreateResample();
    adecode->AddObs(resample);

    //audioplay
    IAudioPlay* audioPlay = CreateAudioPlay();
    resample->AddObs(audioPlay);

    player->demux = de;
    player->adecode = adecode;
    player->vdecode = vdecode;
    player->videoview = view;
    player->resample = resample;
    player->audioplay = audioPlay;
    return player;
}
