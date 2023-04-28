//
// Created by huste on 2023-04-21.
//

#include "Texture.h"
#include "Log.h"
#include "EGL.h"
#include "Shader.h"

class CXTexture:public Texture
{
public:
    Shader sh;
    TextureType type;
    std::mutex mux;
    virtual void Drop()
    {
        mux.lock();
        EGL::Get()->Close();
        sh.Close();
        mux.unlock();
        delete this;
    }

    virtual bool Init(void *win,TextureType type)
    {
        mux.lock();
        EGL::Get()->Close();
        sh.Close();
        this->type = type;
        if(!win)
        {
            mux.unlock();
            LOGE("XTexture Init failed win is NULL");
            return false;
        }
        if(!EGL::Get()->Init(win))
        {
            mux.unlock();
            return false;
        }

        sh.Init((ShaderType)type);
        mux.unlock();
        return true;
    }
    virtual void Draw(unsigned char *data[],int width,int height)
    {
        mux.lock();
        sh.GetTexture(0,width,height,data[0]);  // Y

        if(type == TEXTURE_YUV420P)
        {
            sh.GetTexture(1,width/2,height/2,data[1]);  // U
            sh.GetTexture(2,width/2,height/2,data[2]);  // V
        }
        else
        {
            sh.GetTexture(1,width/2,height/2,data[1], true);  // UV
        }
        sh.Draw();
        EGL::Get()->Draw();
        mux.unlock();
    }

};

Texture *Texture::Create()
{
    return  new CXTexture();
}

void Texture::Drop()
{

}

Texture::Texture()
{

}
