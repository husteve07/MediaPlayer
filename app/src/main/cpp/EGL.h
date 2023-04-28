//
// Created by huste on 2023-04-21.
//

#ifndef MEDIAPLAYER_EGL_H
#define MEDIAPLAYER_EGL_H


class EGL {
public:
    virtual bool Init(void* win) = 0;
    virtual void Close() = 0;
    static EGL* Get();
    virtual void Draw() = 0;
protected:
    EGL(){}

};


#endif //MEDIAPLAYER_EGL_H
