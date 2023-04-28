//
// Created by huste on 2023-04-22.
//

#include "FFmpegResample.h"
#include "Log.h"

extern "C"
{
    #include <libswresample/swresample.h>
}
#include <libavcodec/avcodec.h>

bool FFmpegResample::Open(Parameter in, Parameter out)
{
    Close();
    mux.lock();
    actx = swr_alloc();
    actx = swr_alloc_set_opts(actx,
                              av_get_default_channel_layout(out.channels),
                              AV_SAMPLE_FMT_S16, out.sample_rate,
                              av_get_default_channel_layout(in.avcodec_para->channels),
            (AVSampleFormat)in.avcodec_para->format, in.avcodec_para->sample_rate, 0,0);
    int re = swr_init(actx);
    if(re!= 0)
    {
        mux.unlock();
        LOGE("swr_init fialed");
        return false;
    }
    else
    {
        LOGI("swr_init success");
    }
    outChannels = in.avcodec_para->channels;
    outFormat = AV_SAMPLE_FMT_S16;
    mux.unlock();
    return true;
}

Data FFmpegResample::Resample(Data indata)
{
    if(indata.size <=0 || !indata.data) return Data();
    mux.lock();
    if(!actx)
    {
        mux.unlock();
        return Data();
    }
    AVFrame* frame = (AVFrame*) indata.data;

    Data out;
    int outsize = outChannels * frame->nb_samples * av_get_bytes_per_sample((AVSampleFormat) outFormat);
    if(outsize <=0) return Data();
    out.Alloc(outsize);
    uint8_t* outArr[2] = {0};
    outArr[0] = out.data;

    int len = swr_convert(actx, outArr, frame->nb_samples,(const uint8_t**)frame->data, frame->nb_samples);
    if(len <=0)
    {
        mux.unlock();
        out.Drop();
        return Data();
    }
    out.pts = indata.pts;
    mux.unlock();
    //LOGE("swrConvert Success %d", len);
    return out;

}

void FFmpegResample::Close()
{
    mux.lock();
    if(actx)
    {
        swr_free(&actx);
    }
    mux.unlock();

}
