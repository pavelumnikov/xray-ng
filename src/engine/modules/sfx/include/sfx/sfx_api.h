// This file is a part of xray-ng engine
//

#pragma once

#include "sfx/sfx_environment.h"

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, sfx)

//-----------------------------------------------------------------------------------------------------------
class XR_NON_VIRTUAL api
{
public:
    XR_PURE_VIRTUAL_DESTRUCTOR(api);
    virtual utils::shared_ptr<environment> create_environment() = 0;
    virtual void switch_to_environment(utils::shared_ptr<environment> const& env) = 0;
}; // class api

XR_NAMESPACE_END(xr, sfx)
//-----------------------------------------------------------------------------------------------------------
