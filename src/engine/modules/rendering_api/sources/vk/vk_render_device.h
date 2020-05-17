// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/threading/spin_wait.h"
#include "corlib/memory/memory_aligned_allocator.h"
#include "rendering_api/common.h"
#include "../base_render_device.h"
#include "vk_mem_proxy.h"
#define GLFW_EXPOSE_NATIVE_WIN32
#include "GLFW/glfw3.h"

// forward declaration
VK_DEFINE_HANDLE(VmaAllocator)

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, rendering_api, vk)

//-----------------------------------------------------------------------------------------------------------
struct queue_family_indices
{
    int32_t graphics_family;
    int32_t present_family;
    int32_t copy_family;

    XR_CONSTEXPR_CPP14_OR_INLINE queue_family_indices() XR_NOEXCEPT;
    bool is_complete() const XR_NOEXCEPT;
}; // struct queue_family_indices

//-----------------------------------------------------------------------------------------------------------
/**
 */
XR_CONSTEXPR_CPP14_OR_INLINE queue_family_indices::queue_family_indices() XR_NOEXCEPT
    : graphics_family { -1 }
    , present_family { -1 }
    , copy_family { -1 }
{}

//-----------------------------------------------------------------------------------------------------------
/**
 */
inline bool queue_family_indices::is_complete() const XR_NOEXCEPT
{
    return graphics_family >= 0 && present_family >= 0 && copy_family >= 0;
}

//-----------------------------------------------------------------------------------------------------------
class XR_ALIGNAS(64) vk_render_device : public base_render_device
{
public:
    vk_render_device(memory::base_allocator& alloc);
    virtual ~vk_render_device();

    // Implementation base_render_device

    virtual void initialize(bool enable_debug_layer) override;
    virtual void shutdown() override;

    // Implementation render_device

    virtual memory::base_allocator& get_allocator() override;

private:
    typedef threading::spin_wait_fairness copy_queue_lock;

    static XR_CONSTEXPR_CPP14_OR_CONST size_t timestamp_query_count = 1024;
    static XR_CONSTEXPR_CPP14_OR_CONST size_t occlusion_query_count = 1024;

    bool enumerate_extensions(bool enable_debug_layer, utils::vector<const char*>& extensions);
    VkResult create_instance(VkApplicationInfo const& app_info, bool enable_validation_layers,
        utils::vector<const char*> const& extensions);
    void register_debugging_layers();

    void initialize_memory_allocation();
    void shutdown_memory_allocation();

    memory::base_allocator& m_allocator;
    memory::aligned_allocator<16> m_cpu_aligned_allocator;
    memory::proxy::eastl_proxy_allocator m_proxy_allocator;
    memory_proxy_allocator m_vk_proxy_allocator;

    VkInstance m_instance;
    VkPhysicalDevice m_active_gpu;
    VkPhysicalDeviceProperties2* m_active_gpu_properties;
    VkDevice m_device;
    VmaAllocator m_vma_allocator;
    struct NullDescriptors* m_null_descriptors;
    char* m_name;
    struct gpu_settings* m_gpu_settings;
    struct shader_macro* m_builtin_shader_defines;
    struct gpu_cap_bits* m_cap_bits;
    uint32_t m_linked_node_count : 4;
    uint32_t m_gpu_mode : 3;
    uint32_t m_shader_target : 4;
    uint32_t m_api : 5;
    uint32_t m_enable_gpu_based_validation : 1;
    uint32_t m_builtin_shader_defines_count;
}; // class context

XR_NAMESPACE_END(xr, rendering_api, vk)
//-----------------------------------------------------------------------------------------------------------
