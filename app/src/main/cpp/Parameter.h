//
// Created by huste on 2023-04-21.
//

#ifndef MEDIAPLAYER_PARAMETER_H
#define MEDIAPLAYER_PARAMETER_H

struct AVCodecParameters;
class Parameter
{
public:
    AVCodecParameters *avcodec_para = 0;
    int channels = 2;
    int sample_rate = 44100;

};


#endif //MEDIAPLAYER_PARAMETER_H
