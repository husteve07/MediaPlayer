//
// Created by huste on 2023-04-19.
//

#ifndef MEDIAPLAYER_DATA_H
#define MEDIAPLAYER_DATA_H
enum DataType
{
    AVPACKET_TYPE = 0,
    UCHAR_TYPE = 1
};

struct Data
{
    int type = 0;
    int pts = 0;
    unsigned char* data = 0;
    unsigned char* datas[8] = {0};
    bool isAudio = false;
    int size = 0;
    int width = 0;
    int height = 0;
    int format = 0;
    bool Alloc(int size, const char* data = 0);
    void Drop();
};


#endif //MEDIAPLAYER_DATA_H
