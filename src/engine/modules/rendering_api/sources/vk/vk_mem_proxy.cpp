// This file is a part of xray-ng engine
//

#include "vk_mem_proxy.h"
#include "corlib/memory/memory_aligned_helpers.h"

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, rendering_api, vk)

//-----------------------------------------------------------------------------------------------------------
/**
 */
memory_proxy_allocator::memory_proxy_allocator(memory::base_allocator& alloc)
    : m_allocator { alloc }
    , m_allocation_callbacks {}
{
    m_allocation_callbacks.pfnAllocation = &malloc_memory;
    m_allocation_callbacks.pfnReallocation = &realloc_memory;
    m_allocation_callbacks.pfnFree = &free_memory;
    m_allocation_callbacks.pfnInternalAllocation = &notify_alloc_memory;
    m_allocation_callbacks.pfnInternalFree = &notify_free_memory;
    m_allocation_callbacks.pUserData = this;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
const VkAllocationCallbacks& memory_proxy_allocator::get_allocation_callbacks() const
{
    return m_allocation_callbacks;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
void* memory_proxy_allocator::malloc_memory(void* user_data, size_t size, size_t align,
    VkSystemAllocationScope scope)
{
    XR_DEBUG_ASSERTION_MSG(user_data != nullptr, "invalid user data pointer");
    memory_proxy_allocator* self = reinterpret_cast<memory_proxy_allocator*>(user_data);
    return memory::call_aligned_malloc(self->m_allocator, size, align
        XR_DEBUG_PARAMETERS_DESCRIPTION_DEFINITION("Vulkan Malloc") XR_DEBUG_PARAMETERS_DEFINITION);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
void* memory_proxy_allocator::realloc_memory(void* user_data, void* ptr, size_t size, size_t align,
    VkSystemAllocationScope scope)
{
    XR_DEBUG_ASSERTION_MSG(user_data != nullptr, "invalid user data pointer");
    memory_proxy_allocator* self = reinterpret_cast<memory_proxy_allocator*>(user_data);
    return memory::call_aligned_realloc(self->m_allocator, ptr, size, align
        XR_DEBUG_PARAMETERS_DESCRIPTION_DEFINITION("Vulkan Realloc") XR_DEBUG_PARAMETERS_DEFINITION);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
void memory_proxy_allocator::free_memory(void* user_data, void* ptr)
{
    if(!ptr) return;
    XR_DEBUG_ASSERTION_MSG(user_data != nullptr, "invalid user data pointer");
    memory_proxy_allocator* self = reinterpret_cast<memory_proxy_allocator*>(user_data);
    memory::call_aligned_free(self->m_allocator, ptr XR_DEBUG_PARAMETERS_DEFINITION);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
void memory_proxy_allocator::notify_alloc_memory(void* user_data, size_t size,
    VkInternalAllocationType type, VkSystemAllocationScope scope)
{
    // TODO: may be implement
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
void memory_proxy_allocator::notify_free_memory(void* user_data, size_t size,
    VkInternalAllocationType type, VkSystemAllocationScope scope)
{
    // TODO: may be implement
}

XR_NAMESPACE_END(xr, rendering_api, vk)
//-----------------------------------------------------------------------------------------------------------