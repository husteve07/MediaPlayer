//
// Created by huste on 2023-04-20.
//

#include "IObserver.h"

void IObserver::AddObs(IObserver *obs)
{
    if(!obs) return;
    mux.lock();
    obss.push_back(obs);
    mux.unlock();
}

void IObserver::Notify(Data data)
{
    mux.lock();
    for(int i = 0; i < obss.size(); i++)
    {
        obss[i]->Update(data);
    }
    mux.unlock();
}
