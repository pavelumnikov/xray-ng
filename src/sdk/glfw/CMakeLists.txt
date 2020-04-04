cmake_minimum_required(VERSION 3.10)

project(glfw)

if (MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /bigobj")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
endif(MSVC)

set(GLFW_HEADERS 
	"include/GLFW/glfw3.h"
	"include/GLFW/glfw3native.h")

set(GLFW_SOURCES
	"src/context.c"
	"src/init.c"
	"src/input.c"
	"src/monitor.c"
	"src/vulkan.c"
	"src/window.c")

if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	set(GLFW_SOURCES
		${GLFW_SOURCES}
		"src/win32_platform.h"
		"src/win32_joystick.h"
        "src/wgl_context.h"
		"src/egl_context.h"
		"src/osmesa_context.h"
		"src/win32_init.c"
		"src/win32_joystick.c"
		"src/win32_monitor.c"
		"src/win32_time.c"
		"src/win32_thread.c"
		"src/win32_window.c"
        "src/wgl_context.c"
		"src/egl_context.c"
		"src/osmesa_context.c")
endif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")

set(SOURCES ${GLFW_HEADERS} ${GLFW_SOURCES})
set(OPTIONS generic:purec=yes)
xrng_engine_add_thirdparty(${PROJECT_NAME} STATIC OPTIONS NoDependency SOURCES)

if(WIN32)
	target_compile_definitions(${PROJECT_NAME} PRIVATE _GLFW_WIN32)
endif(WIN32)

if(UNIX)
	target_compile_definitions(${PROJECT_NAME} PRIVATE _GLFW_X11)
endif(UNIX)

set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER ${XR_PROJECT_PREFIX})