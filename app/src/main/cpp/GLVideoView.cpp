//
// Created by huste on 2023-04-21.
//

#include "GLVideoView.h"
#include "Texture.h"
#include "Log.h"
void GLVideoView::SetRender(void *win)
{
    view = win;
}
void GLVideoView::Render(Data data)
{

    if(!view) return;
    if(!txt)
    {
        txt = Texture::Create();

        txt->Init(view,(TextureType)data.format);
    }
    txt->Draw(data.datas,data.width,data.height);
}

void GLVideoView::Close()
{
    mux.lock();
    if(txt)
    {
        txt->Drop();
        txt = 0;
    }
    mux.unlock();
}
