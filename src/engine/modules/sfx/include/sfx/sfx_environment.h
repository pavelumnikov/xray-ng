// This file is a part of xray-ng engine
//

#pragma once

#include "sfx/sfx_event.h"
#include "corlib/utils/string_view.h"
#include "corlib/utils/ptr.h"

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, sfx)

//-----------------------------------------------------------------------------------------------------------
class XR_NON_VIRTUAL environment
{
public:
    XR_PURE_VIRTUAL_DESTRUCTOR(environment);
    virtual utils::shared_ptr<event> create_event(utils::string_view name) = 0;
    virtual void stop_all_events() = 0;
    virtual uint32_t current_playing_events() = 0;
}; // class environment

XR_NAMESPACE_END(xr, sfx)
//-----------------------------------------------------------------------------------------------------------
