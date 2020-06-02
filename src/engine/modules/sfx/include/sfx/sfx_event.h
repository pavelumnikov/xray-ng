// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/types.h"

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, sfx)

//-----------------------------------------------------------------------------------------------------------
class XR_NON_VIRTUAL event
{
public:
    XR_PURE_VIRTUAL_DESTRUCTOR(event);
    virtual void start() = 0;
    virtual void stop_fadeout() = 0;
    virtual void stop() = 0;
}; // class event

XR_NAMESPACE_END(xr, sfx)
//-----------------------------------------------------------------------------------------------------------