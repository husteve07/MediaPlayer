//
// Created by huste on 2023-04-19.
//

#include "Data.h"
extern "C"
{
#include <libavformat/avformat.h>
}
void Data::Drop()
{
    if(!data)
        return;
    if(type == AVPACKET_TYPE)
        av_packet_free((AVPacket **) &data);
    else
        delete data;
    data = 0;
    size = 0;
}

bool Data::Alloc(int size, const char* d)
{
    Drop();
    type = UCHAR_TYPE;
    if (size <=0)
        return false;
    this->data = new unsigned char[size];
    if (!this->data) return false;
    if(d)
    {
        memcpy(this->data, d, size);
    }
    this->size = size;
    return true;
}
