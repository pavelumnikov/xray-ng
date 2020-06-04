// This file is a part of xray-ng engine
//

#include "extension/context.h"
#include "task/parallel_init_task.h"

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, extension)

//-----------------------------------------------------------------------------------------------------------
/**
 */
context::context(memory::base_allocator& alloc)
    : m_allocator { alloc }
    , m_proxy_allocator { m_allocator }
    , m_subsystems { m_proxy_allocator }
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
    size_t count = m_subsystems.size();

    pvoid view_map = XR_STACK_ALLOCATE_MEMORY(sizeof(task::parallel_init_task) * count);
    utils::array_view<task::parallel_init_task> t(view_map, count);

    for(size_t i = 0; i < count; ++i)
    {
        auto ptr = m_subsystems[i].ptr;
        memory::call_emplace_construct(&t[i], ptr); // for async init
    }

    ctx.run_subtasks_and_yield(tasks::task_group::get_default_group(), t.get_raw_data(), t.size());
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
