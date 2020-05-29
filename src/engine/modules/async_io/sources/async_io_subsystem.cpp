// This file is a part of xray-ng engine
//

#include "async_io/async_io_subsystem.h"
#include "file_api_win32.h"
#include "corlib/memory/allocator_macro.h"

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, async_io)

//-----------------------------------------------------------------------------------------------------------
/**
 */
async_io_subsystem::async_io_subsystem(extension::context* ctx, memory::base_allocator& alloc)
    : extension::subsystem(ctx)
    , m_allocator(alloc)
    , m_file_api()
{
    initialize_self();
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
async_io_subsystem::~async_io_subsystem()
{
    destroy_self();
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
bool async_io_subsystem::initialize_async(tasks::execution_context& ctx)
{
    return true;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
void async_io_subsystem::tick_async(tasks::execution_context& ctx, float delta)
{}

//-----------------------------------------------------------------------------------------------------------
/**
 */
file_api& async_io_subsystem::get_file_api()
{
    XR_DEBUG_ASSERTION_MSG(m_file_api != nullptr, "file api not initialized");
    return *m_file_api;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
void async_io_subsystem::initialize_self()
{
    m_file_api = XR_ALLOCATE_OBJECT_T(m_allocator, file_api_win32, "FileAPI for Win32")(m_allocator);
    XR_DEBUG_ASSERTION_MSG(m_file_api != nullptr, "failed to initialize FileAPI");
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
void async_io_subsystem::destroy_self()
{
    XR_DEALLOCATE_MEMORY_T(m_allocator, m_file_api);
}

XR_NAMESPACE_END(xr, async_io)
//-----------------------------------------------------------------------------------------------------------