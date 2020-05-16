cmake_minimum_required(VERSION 3.10)

##

function (xrng_engine_add_vulkan ModuleName)
	## Predefine path to Vulkan
	set(VULKAN_PATH $ENV{VK_SDK_PATH} CACHE INTERNAL "Vulkan path")
	set(VULKAN_INCLUDE_PATH "${VULKAN_PATH}/Include" CACHE INTERNAL "Vulkan include path")
	set(VULKAN_LIBRARY_PATH "${VULKAN_PATH}/Lib" CACHE INTERNAL "Vulkan library path")
	## Add private include paths for selected project name
	target_include_directories(${ModuleName} PRIVATE ${VULKAN_INCLUDE_PATH})
	target_link_directories(${ModuleName} PRIVATE ${VULKAN_LIBRARY_PATH})
	target_link_libraries(${ModuleName} vulkan-1)
endfunction(xrng_engine_add_vulkan ModuleName)