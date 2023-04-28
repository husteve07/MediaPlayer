//
// Created by huste on 2023-04-19.
//

#include "FFmpegDemux.h"
#include "Log.h"
extern "C"{
#include <libavformat/avformat.h>
}

static double r2d(AVRational r)
{
    return r.num == 0 || r.den == 0 ? 0. : (double) r.num/(double) r.den;
}

bool FFmpegDemux::Open(const char *url)
{
    Close();
    mux.lock();
    int re = avformat_open_input(&ic, url, 0 ,0);
    if(re != 0)
    {
        mux.unlock();
        char buf[1024] = {0};
        av_strerror(re,buf,sizeof(buf));
        LOGE("open ffmpeg demux %s failed", url);
        LOGE("error code %d", re);
        return false;
    }
    LOGI("open ffmpeg demux %s success", url);

    //extract file info
    re = avformat_find_stream_info(ic, 0);
    if(re != 0)
    {
        mux.unlock();
        char buf[1024] = {0};
        av_strerror(re,buf,sizeof(buf));
        LOGE("open avformat_find_stream_info failed");
        LOGE("error code %d", re);
        return false;
    }

    this->totalTime = ic->duration/(AV_TIME_BASE/1000);
    LOGI("total duration(ms) = %d", totalTime);
    mux.unlock();
    GetVideoPara();
    GetAudioPara();
    return true;
}

Data FFmpegDemux::Read()
{
    mux.lock();
    if(!ic) {
        mux.unlock();
        return (Data());
    }
    Data d;
    AVPacket *pkt = av_packet_alloc();
    int re = av_read_frame(ic, pkt);
    if(re != 0)
    {
        mux.unlock();
        av_packet_free(&pkt);
        return Data();
    }
    //LOGI("pack size %d, pts %lld", pkt->size, pkt->pts);
    d.data = (unsigned char*)pkt;
    d.size = pkt->size;
    if(pkt->stream_index == audioStream)
    {
        //LOGE("stream index audio stream :%d", pkt->stream_index);
        d.isAudio = true;
    }
    else if(pkt->stream_index == videoStream)
    {
        //LOGE("stream index video stream :%d", pkt->stream_index);
        d.isAudio = false;
    }
    else
    {
        av_packet_free(&pkt);
        mux.unlock();
        return Data();
    }

    //conver pts
    pkt->pts = pkt->pts * (1000*r2d(ic->streams[pkt->stream_index]->time_base));
    pkt->dts = pkt->dts * (1000*r2d(ic->streams[pkt->stream_index]->time_base));
    d.pts =(int) pkt->pts;
    //LOGE("demux pts %d", d.pts);
    mux.unlock();
    return d;
}

FFmpegDemux::FFmpegDemux()
{
    static bool isFirst = true;
    if(isFirst)
    {
        isFirst = false;
        //register demuxs and decoders and network;
        av_register_all();
        avcodec_register_all();
        avformat_network_init();
        LOGI("Register ffmpeg complete");
    }
}

Parameter FFmpegDemux::GetVideoPara()
{
    mux.lock();
    if(!ic)
    {
        mux.unlock();
        LOGE("GetVideoPara failed");
        return Parameter();
    }
    int re = av_find_best_stream(ic, AVMEDIA_TYPE_VIDEO, -1, -1, 0,0);
    if(re < 0)
    {
        mux.unlock();
        LOGE("av_find_best stream failed");
        return Parameter();
    }
    videoStream = re;
    Parameter para;
    para.avcodec_para = ic->streams[re]->codecpar;
    para.channels = ic->streams[re]->codecpar->channels;
    para.sample_rate = ic->streams[re]->codecpar->sample_rate;
    mux.unlock();
    return para;
}

Parameter FFmpegDemux::GetAudioPara()
{
    if(!ic)
    {
        LOGE("GetAudiopara failed");
        return Parameter();
    }
    int re = av_find_best_stream(ic, AVMEDIA_TYPE_AUDIO,-1,-1,0,0);
    if(re < 0)
    {
        LOGE("av_find_best_stream failed");
        return Parameter();
    }
    audioStream = re;
    Parameter para;
    para.avcodec_para = ic->streams[re]->codecpar;
    return para;
}

void FFmpegDemux::Close()
{
    mux.lock();
    if(ic)
        avformat_close_input(&ic);
    mux.unlock();
}
