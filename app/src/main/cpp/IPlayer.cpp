//
// Created by huste on 2023-04-25.
//

#include "IPlayer.h"
#include "IDecode.h"
#include "IDemux.h"
#include "IVideoView.h"
#include "IResample.h"
#include "IAudioPlay.h"
#include "Log.h"

IPlayer *IPlayer::Get(unsigned char index)
{
    static IPlayer p[256];
    return &p[index];
}

bool IPlayer::Open(const char *path)
{
    Close();
    //unpack
    if(!demux || !demux->Open(path))
    {
        mux.unlock();
        LOGE("demux open failed %s", path);
        return false;
    }
    //decode, might not need it if the native data works out of pack
    if(!vdecode || !vdecode->Open(demux->GetVideoPara(), isHardDecode))
    {
        LOGE("vdecode->open failed %s", path);
        //return false;
    }
    if(!adecode || !adecode->Open(demux->GetAudioPara()))
    {
        LOGE("adecode->open failed %s", path);
        //return false;
    }

    //resample, might not need for the same reason
    //if(outpara.sample_rate <=0)
        outpara = demux->GetAudioPara();
    if(!resample || !resample->Open(demux->GetAudioPara(), outpara))
    {
        LOGE("resample->open failed %s", path);
    }
    mux.unlock();
    return true;
}

bool IPlayer::Start()
{
    mux.lock();
    if(audioplay)
        audioplay->StartPlay(outpara);
    if(vdecode)
        vdecode->Start();
    if(adecode)
        adecode->Start();
    if(!demux || !demux->Start())
    {
        mux.unlock();
        LOGE("demux start failed or null");
        return false;
    }



    Thread::Start();
    mux.unlock();
    return true;
}

void IPlayer::InitView(void *win)
{
    if(videoview)
    {
        videoview->Close();
        videoview->SetRender(win);
    }

}

void IPlayer::Main()
{
    while(!isExit)
    {
        mux.lock();
        if(!audioplay || !vdecode)
        {
            mux.unlock();
            Sleep(2);
            continue;
        }
        int apts = audioplay->pts;
        //LOGE("apts %d", apts);

        vdecode->synPts = apts;
        mux.unlock();
        Sleep(2);
    }

}

void IPlayer::Close()
{
    mux.lock();
    Thread::Stop();

    if(demux)
        demux->Stop();

    if(vdecode)
        vdecode->Stop();
    if(adecode)
        adecode->Stop();

    if(vdecode)
        vdecode->Clear();

    if(adecode)
        adecode->Clear();

    if(audioplay)
        audioplay->Clear();

    if(audioplay)
        audioplay->Close();

    if(videoview)
        videoview->Close();

    if(vdecode)
        vdecode->Close();

    if(adecode)
        adecode->Close();

    if(demux)
        demux->Close();

    mux.unlock();
}



