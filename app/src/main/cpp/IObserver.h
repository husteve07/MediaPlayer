//
// Created by huste on 2023-04-20.
//

#ifndef MEDIAPLAYER_IOBSERVER_H
#define MEDIAPLAYER_IOBSERVER_H

#include "Data.h"
#include "Thread.h"
#include <vector>
#include <mutex>

class IObserver : public Thread
{
public:
    virtual void Update(Data data) {}

    void AddObs(IObserver* obs);

    void Notify(Data data);
protected:
    std::vector<IObserver*> obss;
    std::mutex mux;
};


#endif //MEDIAPLAYER_IOBSERVER_H
