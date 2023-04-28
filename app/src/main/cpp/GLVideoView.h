//
// Created by huste on 2023-04-21.
//

#ifndef MEDIAPLAYER_GLVIDEOVIEW_H
#define MEDIAPLAYER_GLVIDEOVIEW_H


#include "Data.h"
#include "IVideoView.h"
class Texture;
class GLVideoView : public IVideoView
{
public:
    virtual void SetRender(void* win);

    virtual void Render(Data data);

    virtual void Close();
protected:
    void* view = 0;
    Texture *txt = 0;
    std::mutex mux;
};


#endif //MEDIAPLAYER_GLVIDEOVIEW_H
