//
// Created by huste on 2023-04-21.
//

#ifndef MEDIAPLAYER_IVIDEOVIEW_H
#define MEDIAPLAYER_IVIDEOVIEW_H


#include "Data.h"
#include "IObserver.h"

class IVideoView : public IObserver
{
public:
    virtual void SetRender(void* win) = 0;

    virtual void Render(Data data) = 0;

    virtual void Update(Data data);

    virtual void Close() = 0;

};


#endif //MEDIAPLAYER_IVIDEOVIEW_H
