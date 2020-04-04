#include "corlib/sys/exit_handler.h"
//#include "corlib/containers/intrusive_double_list.h"

namespace xr::sys
{

//using exit_handlers_container = containers::intrusive_double_linked_list<
//    exit_handler, exit_handler_node, &exit_handler_node::prev, &exit_handler_node::next>;

//static exit_handlers_container s_exit_handlers;

void register_exit_handler(exit_handler& handler)
{
    //s_exit_handlers.push_back(&handler);
}

void call_exit_handlers()
{

}

} // namespace xr::sys
