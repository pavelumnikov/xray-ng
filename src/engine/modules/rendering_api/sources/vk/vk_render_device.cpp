// This file is a part of xray-ng engine
//

#include "vk_render_device.h"
#include "vk_instance.h"
#include "vk_mem_alloc.h"
#include "corlib/memory/allocator_macro.h"
#include "GLFW/glfw3native.h"

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, rendering_api, vk)

//-----------------------------------------------------------------------------------------------------------
static XR_CONSTEXPR_CPP14_OR_CONST char const* the_instance_extensions[] =
{
    VK_KHR_SURFACE_EXTENSION_NAME,

#if defined(VK_USE_PLATFORM_WIN32_KHR)
    VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
#elif defined(VK_USE_PLATFORM_XLIB_KHR)
    VK_KHR_XLIB_SURFACE_EXTENSION_NAME,
#elif defined(VK_USE_PLATFORM_XCB_KHR)
    VK_KHR_XCB_SURFACE_EXTENSION_NAME,
#elif defined(VK_USE_PLATFORM_ANDROID_KHR)
    VK_KHR_ANDROID_SURFACE_EXTENSION_NAME,
#elif defined(VK_USE_PLATFORM_GGP)
    VK_GGP_STREAM_DESCRIPTOR_SURFACE_EXTENSION_NAME,
#elif defined(VK_USE_PLATFORM_VI_NN)
    VK_NN_VI_SURFACE_EXTENSION_NAME,
#endif

    // Debug utils not supported on all devices yet
#ifdef USE_DEBUG_UTILS_EXTENSION
    VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
#else
    VK_EXT_DEBUG_REPORT_EXTENSION_NAME,
#endif

    VK_NV_EXTERNAL_MEMORY_CAPABILITIES_EXTENSION_NAME,
    // To legally use HDR formats
    VK_EXT_SWAPCHAIN_COLOR_SPACE_EXTENSION_NAME,

    // VR Extensions
    VK_KHR_DISPLAY_EXTENSION_NAME,
    VK_EXT_DIRECT_MODE_DISPLAY_EXTENSION_NAME,

    // Multi GPU Extensions
#if VK_KHR_device_group_creation
    VK_KHR_DEVICE_GROUP_CREATION_EXTENSION_NAME,
#endif

#ifndef NX64 
    // Property querying extensions
    VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME,
#endif
};

//-----------------------------------------------------------------------------------------------------------
static XR_CONSTEXPR_CPP14_OR_CONST char const* the_device_extensions[] =
{
	VK_KHR_SWAPCHAIN_EXTENSION_NAME,
	VK_KHR_MAINTENANCE1_EXTENSION_NAME,
	VK_KHR_SHADER_DRAW_PARAMETERS_EXTENSION_NAME,
	VK_EXT_SHADER_SUBGROUP_BALLOT_EXTENSION_NAME,
	VK_EXT_SHADER_SUBGROUP_VOTE_EXTENSION_NAME,
	VK_KHR_DEDICATED_ALLOCATION_EXTENSION_NAME,
	VK_KHR_GET_MEMORY_REQUIREMENTS_2_EXTENSION_NAME,

#ifdef USE_EXTERNAL_MEMORY_EXTENSIONS
	VK_KHR_EXTERNAL_MEMORY_EXTENSION_NAME,
	VK_KHR_EXTERNAL_SEMAPHORE_EXTENSION_NAME,
	VK_KHR_EXTERNAL_FENCE_EXTENSION_NAME,
#if defined(VK_USE_PLATFORM_WIN32_KHR)
	VK_KHR_EXTERNAL_MEMORY_WIN32_EXTENSION_NAME,
	VK_KHR_EXTERNAL_SEMAPHORE_WIN32_EXTENSION_NAME,
	VK_KHR_EXTERNAL_FENCE_WIN32_EXTENSION_NAME,
#endif
#endif

	// Debug marker extension in case debug utils is not supported
#ifndef USE_DEBUG_UTILS_EXTENSION
	VK_EXT_DEBUG_MARKER_EXTENSION_NAME,
#endif

#if defined(VK_USE_PLATFORM_GGP)
	VK_GGP_FRAME_TOKEN_EXTENSION_NAME,
#endif

#if VK_KHR_draw_indirect_count
	VK_KHR_DRAW_INDIRECT_COUNT_EXTENSION_NAME,
#endif

	// Fragment shader interlock extension to be used for ROV type functionality in Vulkan
#if VK_EXT_fragment_shader_interlock
	VK_EXT_FRAGMENT_SHADER_INTERLOCK_EXTENSION_NAME,
#endif

	// NVIDIA Specific Extensions
#ifdef USE_NV_EXTENSIONS
	VK_NVX_DEVICE_GENERATED_COMMANDS_EXTENSION_NAME,
#endif

	// AMD Specific Extensions
	VK_AMD_DRAW_INDIRECT_COUNT_EXTENSION_NAME,
	VK_AMD_SHADER_BALLOT_EXTENSION_NAME,
	VK_AMD_GCN_SHADER_EXTENSION_NAME,

	// Multi GPU Extensions
#if VK_KHR_device_group
	VK_KHR_DEVICE_GROUP_EXTENSION_NAME,
#endif

	// Bindless & None Uniform access Extensions
	VK_EXT_DESCRIPTOR_INDEXING_EXTENSION_NAME,
	// Descriptor Update Template Extension for efficient descriptor set updates
	VK_KHR_DESCRIPTOR_UPDATE_TEMPLATE_EXTENSION_NAME,
	
#ifdef ENABLE_RAYTRACING
    // Raytracing
	VK_NV_RAY_TRACING_EXTENSION_NAME,
#endif
};

static XR_CONSTEXPR_CPP14_OR_CONST eastl::array<const char*> the_validation_layers =
{
    { "VK_LAYER_LUNARG_standard_validation" }
};

//-----------------------------------------------------------------------------------------------------------
static bool check_validation_layer_support(memory::proxy::eastl_proxy_allocator& alloc)
{
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    utils::vector<VkLayerProperties> available_layers(layerCount, alloc);
    vkEnumerateInstanceLayerProperties(&layerCount, available_layers.data());

    for(const char* layer_name : the_validation_layers)
    {
        bool layer_found = false;
        for(const auto& layer_properties : available_layers)
        {
            if(strcmp(layer_name, layer_properties.layerName) == 0)
            {
                layer_found = true;
                break;
            }
        }

        if(!layer_found)
            return false;
    }

    return true;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
vk_render_device::vk_render_device(memory::base_allocator& alloc)
    : m_allocator { alloc }
    , m_cpu_aligned_allocator { alloc }
    , m_proxy_allocator { alloc }
    , m_vk_proxy_allocator { alloc }
    , m_instance {}
{}

//-----------------------------------------------------------------------------------------------------------
/**
 */
vk_render_device::~vk_render_device()
{}

//-----------------------------------------------------------------------------------------------------------
/**
 */
void vk_render_device::initialize(bool enable_debug_layer)
{
    VkApplicationInfo app_info = {};
    app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    app_info.pNext = nullptr;
    app_info.pApplicationName = "xray-ng application";
    app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    app_info.pEngineName = "xray-ng";
    app_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    app_info.apiVersion = VK_API_VERSION_1_1;

    // Create Vulkan binding
    VkResult result = initialize_instance_table();
    if(!VK_CHECK_RESULT(result))
    {
        XR_DEBUG_ASSERTION(false);
        return; // TODO: error handling?
    }

    // Enumerate available extensions:
    utils::vector<const char*> extension_names(m_proxy_allocator);
    bool validation_enabled = enumerate_extensions(true, extension_names);

    // Create instance
    result = create_instance(app_info, validation_enabled, extension_names);
    if(!VK_CHECK_RESULT(result))
    {
        XR_DEBUG_ASSERTION(false);
        return; // TODO: error handling?
    }

    // Register validation layer callback
    register_debugging_layers();

    // Memory allocator
    initialize_memory_allocation();

    // Surface creation
    //VkAllocationCallbacks const& alloc = m_vk_proxy_allocator.get_allocation_callbacks();
    //glfwCreateWindowSurface(m_instance, window, &alloc, &m_surface);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
void vk_render_device::shutdown()
{

}

//-----------------------------------------------------------------------------------------------------------
/**
 */
memory::base_allocator& vk_render_device::get_allocator()
{
    return m_allocator;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
bool vk_render_device::enumerate_extensions(bool enable_debug_layer, utils::vector<const char*>& extension_names)
{
    uint32_t extensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
    utils::vector<VkExtensionProperties> extensions(extensionCount, m_proxy_allocator);
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

    extension_names.push_back(VK_KHR_SURFACE_EXTENSION_NAME);
    extension_names.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
    extension_names.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

    bool enable_validation_layers = enable_debug_layer;
    if(enable_debug_layer && !check_validation_layer_support(m_proxy_allocator))
    {
        enable_validation_layers = false;
    }
    else if(enable_validation_layers)
    {
        extension_names.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
    }

    return enable_validation_layers;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
VkResult vk_render_device::create_instance(VkApplicationInfo const& app_info, bool enable_validation_layers,
    utils::vector<const char*> const& ext)
{
    XR_CONSTEXPR_CPP14_OR_CONST uint8_t max_instance_extensions = 64;
    char const* instance_extensions[max_instance_extensions] = {};

    uint32_t layer_count {};
    uint32_t ext_count {};

    VkResult result = VK_SUCCESS;
    result = vkEnumerateInstanceLayerProperties(&layer_count, nullptr);
    if(!VK_CHECK_RESULT(result))
        return result;

    result = vkEnumerateInstanceExtensionProperties(nullptr, &ext_count, nullptr);
    if(!VK_CHECK_RESULT(result))
        return result;

    VkLayerProperties* layers = XR_STACK_ALLOCATE_ARRAY_T(VkLayerProperties, layer_count);
    result = vkEnumerateInstanceLayerProperties(&layer_count, layers);
    if(!VK_CHECK_RESULT(result))
        return result;

    VkExtensionProperties* exts = XR_STACK_ALLOCATE_ARRAY_T(VkExtensionProperties, ext_count);
    result = vkEnumerateInstanceExtensionProperties(NULL, &ext_count, exts);
    if(!VK_CHECK_RESULT(result))
        return result;

#if VK_HEADER_VERSION >= 108
    VkValidationFeaturesEXT validation_features_ext = { VK_STRUCTURE_TYPE_VALIDATION_FEATURES_EXT };
    VkValidationFeatureEnableEXT enabled_validation_features[] =
    {
        VK_VALIDATION_FEATURE_ENABLE_GPU_ASSISTED_EXT,
    };

    // TODO: add GPU-based validation
#endif

    VkInstanceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;

#if VK_HEADER_VERSION >= 108
    create_info.pNext = &validation_features_ext;
#endif

    createInfo.pApplicationInfo = &app_info;
    createInfo.enabledExtensionCount = static_cast<uint32_t>(ext.size());
    createInfo.ppEnabledExtensionNames = ext.data();
    createInfo.enabledLayerCount = 0;
    if(enable_validation_layers)
    {
        createInfo.enabledLayerCount = static_cast<uint32_t>(the_validation_layers.size());
        createInfo.ppEnabledLayerNames = the_validation_layers.data();
    }

    VkAllocationCallbacks const& alloc = m_vk_proxy_allocator.get_allocation_callbacks();
    result = vkCreateInstance(&createInfo, &alloc, &m_instance);
    result = initialize_device_table(m_instance);
    return result;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
void vk_render_device::register_debugging_layers()
{
    //VkDebugReportCallbackCreateInfoEXT create_info = {};
    //create_info.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
    //create_info.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT;
    //create_info.pfnCallback = debugCallback;
    //res = CreateDebugReportCallbackEXT(m_instance, &create_info, nullptr, &callback);
    //assert(res == VK_SUCCESS);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
void vk_render_device::initialize_memory_allocation()
{
    VmaAllocatorCreateInfo create_info = { 0 };
    create_info.device = m_device;
    create_info.physicalDevice = m_active_gpu;
    // TODO: add RenderDoc support

    VmaVulkanFunctions vma_functions = {};
    vma_functions.vkAllocateMemory = vkAllocateMemory;
    vma_functions.vkBindBufferMemory = vkBindBufferMemory;
    vma_functions.vkBindImageMemory = vkBindImageMemory;
    vma_functions.vkCreateBuffer = vkCreateBuffer;
    vma_functions.vkCreateImage = vkCreateImage;
    vma_functions.vkDestroyBuffer = vkDestroyBuffer;
    vma_functions.vkDestroyImage = vkDestroyImage;
    vma_functions.vkFreeMemory = vkFreeMemory;
    vma_functions.vkGetBufferMemoryRequirements = vkGetBufferMemoryRequirements;
    vma_functions.vkGetBufferMemoryRequirements2KHR = vkGetBufferMemoryRequirements2KHR;
    vma_functions.vkGetImageMemoryRequirements = vkGetImageMemoryRequirements;
    vma_functions.vkGetImageMemoryRequirements2KHR = vkGetImageMemoryRequirements2KHR;
    vma_functions.vkGetPhysicalDeviceMemoryProperties = vkGetPhysicalDeviceMemoryProperties;
    vma_functions.vkGetPhysicalDeviceProperties = vkGetPhysicalDeviceProperties;
    vma_functions.vkMapMemory = vkMapMemory;
    vma_functions.vkUnmapMemory = vkUnmapMemory;
    vma_functions.vkFlushMappedMemoryRanges = vkFlushMappedMemoryRanges;
    vma_functions.vkInvalidateMappedMemoryRanges = vkInvalidateMappedMemoryRanges;
    vma_functions.vkCmdCopyBuffer = vkCmdCopyBuffer;

    create_info.pVulkanFunctions = &vma_functions;
    vmaCreateAllocator(&create_info, &m_vma_allocator);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
void vk_render_device::shutdown_memory_allocation()
{
    vmaDestroyAllocator(m_vma_allocator);
}

XR_NAMESPACE_END(xr, rendering_api, vk)
//-----------------------------------------------------------------------------------------------------------