//
// Created by huste on 2023-04-21.
//

#ifndef MEDIAPLAYER_TEXTURE_H
#define MEDIAPLAYER_TEXTURE_H
enum TextureType
{
    TEXTURE_YUV420P= 0,
    TEXTURE_NV12 = 25,
    TEXTURE_NV21 = 26
};

class Texture
{
public:
    static Texture* Create();
    virtual bool Init(void *win, TextureType type = TEXTURE_NV12) = 0;
    virtual void Draw(unsigned char* data[], int width, int height) = 0;
    virtual void Drop() = 0;
    virtual ~Texture(){};
protected:
    Texture();
};


#endif //MEDIAPLAYER_TEXTURE_H
