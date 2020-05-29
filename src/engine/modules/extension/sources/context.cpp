// This file is a part of xray-ng engine
//

#include "extension/context.h"

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, extension)

//-----------------------------------------------------------------------------------------------------------
/**
 */
context::context(memory::base_allocator& alloc)
    : m_allocator { alloc }
    , m_subsystems {}
{}

//-----------------------------------------------------------------------------------------------------------
/**
 */
context::~context()
{
    // Loop in reverse registration order to avoid dependency conflicts
    for(size_t i = m_subsystems.size() - 1; i > 0; --i)
        m_subsystems[i].ptr.reset();
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
bool context::initialize_async(tasks::execution_context& ctx)
{
    bool result = true;
    for(size_t i = 0; i < m_subsystems.size(); ++i)
    {
        if(!m_subsystems[i].ptr->initialize_async(ctx))
        {
            result = false;
        }
    }

    ctx.yield();
    return result;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
void context::tick_async(tasks::execution_context& ctx, tick_group group, float delta)
{
    for(size_t i = 0; i < m_subsystems.size(); ++i)
    {
        if(m_subsystems[i].group != group)
            continue;

        m_subsystems[i].ptr->tick_async(ctx, delta);
    }

    ctx.yield();
}

XR_NAMESPACE_END(xr, extension)
//-----------------------------------------------------------------------------------------------------------
