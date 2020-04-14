// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/threading/event.h"

//-----------------------------------------------------------------------------------------------------------
namespace xr::tasks
{

using wait_result_enum = threading::event_wait_result;

//-----------------------------------------------------------------------------------------------------------
class event : private threading::event
{
public:
    event(bool initial_state = false);
    void set(bool value);
    wait_result_enum wait_timeout(sys::tick timeout);
};

//-----------------------------------------------------------------------------------------------------------
/**
*/
inline event::event(bool initial_state) 
    : threading::event(initial_state)
{}

//-----------------------------------------------------------------------------------------------------------
/**
*/
inline void event::set(bool value)
{
    threading::event::set(value);
}

} // namespace xr::tasks
//-----------------------------------------------------------------------------------------------------------
