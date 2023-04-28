//
// Created by huste on 2023-04-19.
//

#include "IDemux.h"
#include "Log.h"
void IDemux::Main()
{
    while(!isExit)
    {
        Data d = Read();
        if(d.size > 0)
            Notify(d);
        //LOGI("IDemux Read %d", d.size);
        //if(d.size <=0)break;

    }
}
