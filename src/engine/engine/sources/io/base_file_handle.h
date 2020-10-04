// This file is a part of xray-ng engine
//

#pragma once

#include "engine/linkage.h"
#include "corlib/memory/memory_allocator_base.h"
#include "corlib/memory/buffer_ref.h"
#include "corlib/threading/interlocked.h"

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, engine, io)

//-----------------------------------------------------------------------------------------------------------
class XR_NON_VIRTUAL base_file_handle
{
public:
    virtual XR_IO_API ~base_file_handle();

    virtual XR_IO_API size_t tell();
    virtual XR_IO_API bool seek(ssize_t const pos);
    virtual XR_IO_API bool seek_from_end(ssize_t const pos_relative_to_end);
    virtual XR_IO_API bool read(memory::buffer_ref ref, size_t bytes_to_read);
    virtual XR_IO_API bool write(memory::buffer_ref ref, size_t bytes_to_write);
    virtual XR_IO_API bool flush(bool const full_flush = false);
    virtual XR_IO_API bool truncate(size_t new_size);
    virtual XR_IO_API size_t size();

    memory::base_allocator& allocator();

    template<threading::memory_order Order>
    void add_user_reference();

    template<threading::memory_order Order>
    void remove_user_reference();

protected:
    base_file_handle(memory::base_allocator& alloc);

private:
    uint32_t m_user_references;
    memory::base_allocator& m_allocator;
}; // class enviroment_effect

//-----------------------------------------------------------------------------------------------------------
/**
 */
inline memory::base_allocator& base_file_handle::allocator()
{
    return m_allocator;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<threading::memory_order Order>
inline void base_file_handle::add_user_reference()
{
#if defined(DEBUG)
    if (m_user_references == UINT32_MAX)
    {
        XR_DEBUG_ASSERTION_MSG(false, "invalid renferece count value; already max uint32_t");
    }
#endif

    (void)threading::atomic_fetch_add<Order>(m_user_references, 1);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<threading::memory_order Order>
inline void base_file_handle::remove_user_reference()
{
#if defined(DEBUG)
    if (!m_user_references)
    {
        XR_DEBUG_ASSERTION_MSG(false, "invalid renferece count value; already zero");
    }
#endif

    (void)threading::atomic_fetch_sub<Order>(m_user_references, 1);
}

XR_NAMESPACE_END(xr, engine, io)
//-----------------------------------------------------------------------------------------------------------