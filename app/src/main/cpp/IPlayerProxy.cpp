//
// Created by huste on 2023-04-26.
//

#include "IPlayerProxy.h"
#include "FFmpegPlayerBuilder.h"

bool IPlayerProxy::Open(const char *path)
{
    bool re = false;
    mux.lock();
    if(player)
    {
        re = player->Open(path);
    }
    mux.unlock();
    return re;
}

bool IPlayerProxy::Start()
{
    bool re = false;
    mux.lock();
    if(player)
        re = player->Start();
    mux.unlock();
    return re;
}

void IPlayerProxy::InitView(void *win)
{
    mux.lock();
    if(player)
        player->InitView(win);
    mux.unlock();
}

void IPlayerProxy::Init(void* vm)
{
    mux.lock();
    if(vm)
    {
        FFmpegPlayerBuilder::InitHard(vm);
    }
    if(!player)
        player = FFmpegPlayerBuilder::Get()->BuildPlayer();
    mux.unlock();
}

void IPlayerProxy::Close()
{
    mux.lock();
    if(player)
        player->Close();
    mux.unlock();
}

