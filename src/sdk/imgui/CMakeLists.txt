cmake_minimum_required(VERSION 3.10)

## Add modules function
include(${CMAKE_MODULE_PATH}/add_module.cmake)

include(CheckFunctionExists)
include(CheckIncludeFiles)
include(CMakeDependentOption)

project(imgui)

##

set(IMGUI_HEADERS "imgui.h")
source_group("include" FILES ${IMGUI_HEADERS})

##

set(IMGUI_SOURCES
	"imconfig.h"
	"imgui.cpp"
	"imgui_draw.cpp"
	"imgui_internal.h"
	"imgui_widgets.cpp"
	"imstb_rectpack.h"
	"imstb_textedit.h"
	"imstb_truetype.h")
	
source_group("sources" FILES ${IMGUI_SOURCES})

##

set(SOURCES ${IMGUI_HEADERS} ${IMGUI_SOURCES})
set(OPTIONS generic:cpp17=yes generic:noexceptions=yes)
xrng_engine_add_thirdparty(${PROJECT_NAME} STATIC OPTIONS NoDependency SOURCES)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

## For Visual Studio
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER ${XR_PROJECT_PREFIX})