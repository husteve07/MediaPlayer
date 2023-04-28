//
// Created by huste on 2023-04-22.
//

#include "SLAudioPlay.h"
#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>
#include "Log.h"

static SLObjectItf engineSL = NULL;
static SLEngineItf eng = NULL;
static SLObjectItf mix = NULL;
static SLObjectItf player = NULL;
static SLPlayItf Iplay = NULL;
static SLAndroidSimpleBufferQueueItf pcmQueue = NULL;

static SLEngineItf CreateSL()
{

    SLresult re;
    SLEngineItf en;
    re = slCreateEngine(&engineSL, 0,0,0,0,0);
    if(re != SL_RESULT_SUCCESS) return NULL;
    re = (*engineSL)->Realize(engineSL, SL_BOOLEAN_FALSE);
    if(re != SL_RESULT_SUCCESS) return NULL;
    re = (*engineSL)->GetInterface(engineSL, SL_IID_ENGINE, &en);
    if(re != SL_RESULT_SUCCESS) return NULL;
    return en;
}

void SLAudioPlay::PlayCall(void *bufq)
{
    if(!bufq)return;
    SLAndroidSimpleBufferQueueItf bf = (SLAndroidSimpleBufferQueueItf)bufq;
    //LOGE("SLAudioPlay succedd");
    Data d = GetData();
    if(d.size <=0)
    {
        LOGE("GetData() size id 0");
        return;
    }
    if(!buf) return;

    memcpy(buf,d.data,d.size);
    mux.lock();
    if(pcmQueue && (*pcmQueue))
        (*pcmQueue)->Enqueue(pcmQueue,d.data,d.size);
    mux.unlock();
    d.Drop();
}

static void PcmCallback(SLAndroidSimpleBufferQueueItf bf, void *context)
{
    SLAudioPlay *ap = (SLAudioPlay*) context;
    if(!ap)
    {
        LOGE("PcmCallback failed context is null");
        return;
    }
    ap->PlayCall((void*)bf);
}

bool SLAudioPlay::StartPlay(Parameter out)
{
    Close();
    //create Engine
    mux.lock();
    eng = CreateSL();

    if(eng)
    {
        LOGI("CreateSL success");

    }
    else
    {
        mux.unlock();
        LOGE("CreateSL failed");
        return false;
    }

    //initialize audio mixer
    SLresult re = 0;
    re = (*eng)->CreateOutputMix(eng, &mix,0,0,0);
    if(re != SL_RESULT_SUCCESS)
    {
        mux.unlock();
        LOGE("CreateOutPutMix failed");
        return false;
    }
    re = (*mix)->Realize(mix, SL_BOOLEAN_FALSE);

    if(re != SL_RESULT_SUCCESS)
    {
        mux.unlock();
        LOGE("mix realize failed");
        return false;
    }
    SLDataLocator_OutputMix outmix = {SL_DATALOCATOR_OUTPUTMIX, mix};
    SLDataSink audioSink = {&outmix,0};

    //configure audio information
    SLDataLocator_AndroidSimpleBufferQueue queue = {SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE, 10};
    SLDataFormat_PCM pcm = {SL_DATAFORMAT_PCM,
                            (SLuint32 )out.channels,
                            (SLuint32 )out.sample_rate*1000,
                            SL_PCMSAMPLEFORMAT_FIXED_16,
                            SL_PCMSAMPLEFORMAT_FIXED_16,
                            SL_SPEAKER_FRONT_LEFT | SL_SPEAKER_FRONT_RIGHT,
                            SL_BYTEORDER_LITTLEENDIAN};
    SLDataSource ds = {&queue, &pcm};

    //make the audio player
    const SLInterfaceID ids[] = {SL_IID_BUFFERQUEUE};
    const SLboolean req[] = {SL_BOOLEAN_TRUE};
    re = (*eng)->CreateAudioPlayer(eng, &player, &ds, &audioSink, sizeof(ids)/sizeof(SLInterfaceID),ids,req);
    if(re != SL_RESULT_SUCCESS)
    {
        mux.unlock();
        LOGE("Creat audio player failed");
        return false;
    }
    else
    {
        LOGI("Create audio player success");
    }
    (*player)->Realize(player, SL_BOOLEAN_FALSE);
    re = (*player)->GetInterface(player, SL_IID_PLAY, &Iplay);
    if(re != SL_RESULT_SUCCESS)
    {
        mux.unlock();
        LOGE("getinterface success");
        return false;
    }
    re = (*player)->GetInterface(player, SL_IID_BUFFERQUEUE, &pcmQueue);
    if(re != SL_RESULT_SUCCESS)
    {
        mux.unlock();
        LOGE("getinterface pcm queue success");
        return false;
    }

    //configure callback function
    (*pcmQueue)->RegisterCallback(pcmQueue, PcmCallback, this);
    (*Iplay)->SetPlayState(Iplay, SL_PLAYSTATE_PLAYING);
    (*pcmQueue)->Enqueue(pcmQueue,"", 1);
    LOGI("Audio player start play success");
    mux.unlock();
    return true;
}

SLAudioPlay::SLAudioPlay() {
    buf = new unsigned char[1024*1024];

}

SLAudioPlay::~SLAudioPlay() {

    delete buf;
    buf = 0;
}

void SLAudioPlay::Close()
{
    IAudioPlay::Clear();
    mux.lock();
    if(Iplay && (*Iplay))
    {
        (*Iplay)->SetPlayState(Iplay, SL_PLAYSTATE_STOPPED);
    }
    if(pcmQueue && (*pcmQueue))
    {
        (*pcmQueue)->Clear(pcmQueue);
    }
    if(player && (*player))
    {
        (*player)->Destroy(player);
    }

    if(mix && (*mix))
    {
        (*mix)->Destroy(mix);
    }
    if(engineSL && (*engineSL))
    {
        (*engineSL)->Destroy(engineSL);
    }

    engineSL = NULL;
    eng = NULL;
    mix = NULL;
    player = NULL;
    Iplay = NULL;
    pcmQueue = NULL;

    mux.unlock();

}


