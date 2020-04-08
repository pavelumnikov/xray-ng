// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/platform.h"
#include "corlib/etl/containers/intrusive_list_node.h"

//------------------------------------------------------------------------------
namespace xr::sys
{

//------------------------------------------------------------------------------
class XR_NON_VIRTUAL exit_handler 
    : public etl::containers::intrusive_list_node<exit_handler>
{
public:
    virtual void on_exit() = 0;

protected:
    XR_PURE_VIRTUAL_DESTRUCTOR(exit_handler);
}; // class exit_handler

//------------------------------------------------------------------------------
/**
*/
void register_exit_handler(exit_handler& handler);

//------------------------------------------------------------------------------
/**
 * Must be called from main module at exit to call all handlers registered with
 * register_exit_handler() function earlier
*/
void call_exit_handlers();

} // namespace xr::sys
//------------------------------------------------------------------------------