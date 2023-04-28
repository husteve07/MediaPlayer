//
// Created by huste on 2023-04-21.
//

extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavcodec/jni.h>
}
#include "FFmpegDecode.h"
#include "Log.h"

void FFmpegDecode::InitHard(void *vm)
{
    av_jni_set_java_vm(vm,0);
}

bool FFmpegDecode::Open(Parameter para,bool isHard)
{
    Close();
    if(!para.avcodec_para)
    {
        return false;}

    AVCodecParameters *p = para.avcodec_para;

    //1 find decoder(hardware/software)
    AVCodec *cd = avcodec_find_decoder(p->codec_id);
    if(isHard)
    {
        cd = avcodec_find_decoder_by_name("h264_mediacodec");
    }

    if(!cd)
    {

        LOGE("avcodec_find_decoder %d failed!  %d",p->codec_id,isHard);
        return false;
    }
    LOGI("avcodec_find_decoder success %d!",isHard);
    //2 create & copy file context
    mux.lock();
    codec = avcodec_alloc_context3(cd);
    avcodec_parameters_to_context(codec,p);

    codec->thread_count = 8;
    //3 open decoder
    int re = avcodec_open2(codec,0,0);
    if(re != 0)
    {
        mux.unlock();
        char buf[1024] = {0};
        av_strerror(re,buf,sizeof(buf)-1);
        LOGE("%s",buf);
        return false;
    }

    if(codec->codec_type == AVMEDIA_TYPE_VIDEO)
    {
        this->isAudio = false;
    }
    else
    {
        this->isAudio = true;
    }

    mux.unlock();
    LOGI("avcodec_open2 success!");
    return true;
}
bool FFmpegDecode::SendPacket(Data pkt)
{

    if(pkt.size<=0 || !pkt.data)
    {
        return false;
    }
    mux.lock();
    if(!codec)
    {
        mux.unlock();
        return false;
    }
    int re = avcodec_send_packet(codec,(AVPacket*)pkt.data);
    mux.unlock();
    if(re != 0)
    {
        return false;
    }
    return true;
}

//recieve results from thread
Data FFmpegDecode::RecieveFrame()
{
    mux.lock();
    if(!codec)
    {
        mux.unlock();
        return Data();
    }
    if(!frame)
    {
        frame = av_frame_alloc();
    }
    int re = avcodec_receive_frame(codec,frame);
    if(re != 0)
    {
        mux.unlock();
        return Data();
    }
    Data d;
    d.data = (unsigned char *)frame;
    if(codec->codec_type == AVMEDIA_TYPE_VIDEO)
    {
        d.size = (frame->linesize[0] + frame->linesize[1] + frame->linesize[2])*frame->height;
        d.width = frame->width;
        d.height = frame->height;
    }
    else
    {
        //sample characters * single channel samples * number of channels
        d.size = av_get_bytes_per_sample((AVSampleFormat)frame->format)*frame->nb_samples*2;
    }
    d.format = frame->format;
    if(!isAudio)
        //LOGE("data format is %d",frame->format);
    memcpy(d.datas,frame->data,sizeof(d.datas));

    d.pts = frame->pts;
    pts = d.pts;
    mux.unlock();
    return d;
}

void FFmpegDecode::Close()
{
    IDecode::Clear();
    mux.lock();
    pts = 0;
    if(frame)
    {
        av_frame_free(&frame);
    }
    if(codec){
        avcodec_close(codec);
        avcodec_free_context(&codec);
    }
    mux.unlock();
}


