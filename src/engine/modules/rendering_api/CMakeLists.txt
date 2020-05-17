cmake_minimum_required(VERSION 3.10)

## Add modules function
include(${CMAKE_MODULE_PATH}/add_module.cmake)
include(${CMAKE_MODULE_PATH}/modules/include_renderdoc.cmake)

include(CheckFunctionExists)
include(CheckIncludeFiles)
include(CMakeDependentOption)

project(rendering_api)

##

set(RENDERAPI_MODULE_HEADERS
	"include/rendering_api/api.h"
	"include/rendering_api/common.h"
	"include/rendering_api/handle.h"
	"include/rendering_api/render_device.h"
)
	
source_group("include" FILES ${RENDERAPI_MODULE_HEADERS})

##

set(RENDERAPI_MODULE_DESCS_HEADERS
	"include/rendering_api/descs/renderer_desc.h"
)

source_group("include\\descs" FILES ${RENDERAPI_MODULE_DESCS_HEADERS})

##

set(RENDERAPI_MODULE_SOURCES
	"sources/api.cpp"
	"sources/base_render_device.h"
	"sources/debug_renderdoc.cpp"
	"sources/debug_renderdoc.h"
)

source_group("sources" FILES ${RENDERAPI_MODULE_SOURCES})

##

set(RENDERAPI_MODULE_VK_SOURCES
	"sources/vk/vk_include.h"
	"sources/vk/vk_instance.cpp"
	"sources/vk/vk_instance.h"
	"sources/vk/vk_internal_caps.h"
	"sources/vk/vk_mem_alloc.h"
	"sources/vk/vk_mem_proxy.cpp"
	"sources/vk/vk_mem_proxy.h"
	"sources/vk/vk_render_device.cpp"
	"sources/vk/vk_render_device.h"
)

source_group("sources\\vk" FILES ${RENDERAPI_MODULE_VK_SOURCES})

##

set(SOURCES
	${RENDERAPI_MODULE_HEADERS}
	${RENDERAPI_MODULE_DESCS_HEADERS}
	${RENDERAPI_MODULE_SOURCES}
	${RENDERAPI_MODULE_VK_SOURCES}
)

##

set(OPTIONS generic:cpp17=yes generic:noexceptions=yes)
set(DEPENDENCY module:eastl module:glfw module:corlib)

xrng_engine_add_module(${PROJECT_NAME} STATIC OPTIONS DEPENDENCY SOURCES)
xrng_engine_add_renderdoc(${PROJECT_NAME})
## For Visual Studio
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER ${XR_PROJECT_PREFIX})