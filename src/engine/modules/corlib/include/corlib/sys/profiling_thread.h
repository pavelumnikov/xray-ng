// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/sys/thread.h"
#include "corlib/math/color_table.h"

//------------------------------------------------------------------------------
namespace xr::sys
{

//------------------------------------------------------------------------------
/**
*/
void register_thread_id(uint32_t thread_id, eastl::string_view desc, math::color col);

//------------------------------------------------------------------------------
/**
*/
void unregister_thread_id(uint32_t thread_id);


//------------------------------------------------------------------------------
struct raii_register_thread final
{
    explicit raii_register_thread(eastl::string_view desc, math::color col) noexcept
        : m_thread_id(sys::current_thread_id())
    {
        register_thread_id(this->m_thread_id, desc, col);
    }
    
    XR_DECLARE_DELETE_COPY_ASSIGNMENT(raii_register_thread);
    XR_DECLARE_DELETE_MOVE_ASSIGNMENT(raii_register_thread);

    ~raii_register_thread()
    {
        unregister_thread_id(this->m_thread_id);
    }

private:
    uint32_t m_thread_id;
};

} // namespace xr::profiling
//------------------------------------------------------------------------------