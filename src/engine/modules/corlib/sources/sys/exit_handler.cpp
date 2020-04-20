// This file is a part of xray-ng engine
//

#include "corlib/sys/exit_handler.h"

//-----------------------------------------------------------------------------------------------------------
namespace xr::sys
{

using exit_handlers_container = utils::intrusive_list_node<exit_handler>;
static exit_handlers_container s_exit_handlers {};

//-----------------------------------------------------------------------------------------------------------
/**
 */
void register_exit_handler(exit_handler& handler)
{
    s_exit_handlers.append_after_this(handler);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
bool call_exit_handlers()
{
    uint32_t handlers_done = 0;
    bool has_next_handler = s_exit_handlers.has_next();
    if(has_next_handler)
    {
        exit_handler* next_handler = &s_exit_handlers.next();
        for(;;)
        {
            next_handler->on_exit();
            ++handlers_done;

            has_next_handler = next_handler->has_next();

            if(!has_next_handler)
                break;

            next_handler = &next_handler->next();
        }
    }

    return (handlers_done > 0);
}

} // namespace xr::sys
//-----------------------------------------------------------------------------------------------------------