//
// Created by huste on 2023-04-21.
//

#ifndef MEDIAPLAYER_SHADER_H
#define MEDIAPLAYER_SHADER_H
#include <mutex>
enum ShaderType{
    SHADER_YUV420P = 0,
    SHADER_NV12 = 25,
    SHADER_NV21 = 26
};

class Shader
{
public:
    virtual bool Init(ShaderType type = SHADER_NV12);
    virtual void Close();
    virtual void GetTexture(unsigned int index, int width, int height, unsigned char* buf, bool isa = false);
    virtual void Draw();
protected:
    unsigned int vsh = 0;
    unsigned int fsh = 0;
    unsigned int program = 0;
    unsigned int texts[100] = {0};
    std::mutex mux;

};


#endif //MEDIAPLAYER_SHADER_H
