// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/utils/intrusive_list_node.h"

//-----------------------------------------------------------------------------------------------------------
namespace xr::sys
{

//-----------------------------------------------------------------------------------------------------------
class XR_NON_VIRTUAL exit_handler : public utils::intrusive_list_node<exit_handler>
{
public:
    XR_PURE_VIRTUAL_DESTRUCTOR(exit_handler);
    virtual void on_exit() {};
}; // class exit_handler

//-----------------------------------------------------------------------------------------------------------
/**
 */
void register_exit_handler(exit_handler& handler);

//-----------------------------------------------------------------------------------------------------------
/**
 * Must be called from main module at exit to call all handlers registered with
 * register_exit_handler() function earlier
 */
bool call_exit_handlers();

} // namespace xr::sys
//-----------------------------------------------------------------------------------------------------------
