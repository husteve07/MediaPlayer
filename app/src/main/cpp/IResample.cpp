//
// Created by huste on 2023-04-22.
//

#include "IResample.h"
#include "Log.h"

void IResample::Update(Data data)
{
    Data d = this->Resample(data);
    //LOGE("resample data %d", d.size);
    if(d.size>0)
        this->Notify(d);
}
