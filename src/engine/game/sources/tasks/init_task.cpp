// This file is a part of xray-ng engine
//

#include "init_task.h"
#include "async_io/async_io_subsystem.h"
#include "sfx/sfx_subsystem.h"

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, game, task)

//-----------------------------------------------------------------------------------------------------------
/**
 */
init_task::init_task(main::initialize_application_desc const& desc, extension::context& ctx)
    : m_init_desc { desc }
    , m_extension_context { ctx }
{}

//-----------------------------------------------------------------------------------------------------------
/**
 */
void init_task::operator()(tasks::execution_context& context)
{
    m_extension_context.register_subsystem<async_io::async_io_subsystem>(m_init_desc.io_system_allocator);
    m_extension_context.register_subsystem<sfx::sfx_subsystem>(m_init_desc.sfx_allocator);

    auto result = m_extension_context.initialize_async(context);
    if(!result)
    {
        // TODO: add error handling
    }
}

XR_NAMESPACE_END(xr, game, task)
//-----------------------------------------------------------------------------------------------------------