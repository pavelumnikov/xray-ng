// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/utils/vector.h"
#include "vk_include.h"

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, rendering_api, vk)

//-----------------------------------------------------------------------------------------------------------
class memory_proxy_allocator
{
public:
    memory_proxy_allocator(memory::base_allocator& alloc);

    XR_DECLARE_DEFAULT_DESTRUCTOR(memory_proxy_allocator);
    XR_DECLARE_DELETE_COPY_ASSIGNMENT(memory_proxy_allocator);
    XR_DECLARE_DELETE_MOVE_ASSIGNMENT(memory_proxy_allocator);

    const VkAllocationCallbacks& get_allocation_callbacks() const;

private:
    static void* malloc_memory(void* user_data, size_t size, size_t align,
        VkSystemAllocationScope scope);

    static void* realloc_memory(void* user_data, void* ptr, size_t size, size_t align,
        VkSystemAllocationScope scope);

    static void free_memory(void* user_data, void* ptr);

    static void notify_alloc_memory(void* user_data, size_t size, VkInternalAllocationType type, VkSystemAllocationScope scope);

    static void notify_free_memory(void* user_data, size_t size, VkInternalAllocationType type, VkSystemAllocationScope scope);

    memory::base_allocator& m_allocator;
    VkAllocationCallbacks m_allocation_callbacks;
}; // class memory_proxy_allocator

XR_NAMESPACE_END(xr, rendering_api, vk)
//-----------------------------------------------------------------------------------------------------------
