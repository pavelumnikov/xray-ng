cmake_minimum_required(VERSION 3.10)

## Add modules function
include(${CMAKE_MODULE_PATH}/add_module.cmake)

include(CheckFunctionExists)
include(CheckIncludeFiles)
include(CMakeDependentOption)

project(async_io)

##

set(ASYNC_IO_MODULE_HEADERS
	"include/async_io/async_result.h"
	"include/async_io/file_api.h"
)
	
source_group("include" FILES ${ASYNC_IO_MODULE_HEADERS})

##

set(ASYNC_IO_MODULE_SOURCES
	"sources/file_api_win32.cpp"
	"sources/file_api_win32.h"
	"sources/file_async_result_win32.cpp"
	"sources/file_async_result_win32.h"
)

source_group("sources" FILES ${ASYNC_IO_MODULE_SOURCES})

##

set(SOURCES ${ASYNC_IO_MODULE_HEADERS} ${ASYNC_IO_MODULE_SOURCES})

##

set(OPTIONS generic:cpp17=yes generic:noexceptions=yes)
set(DEPENDENCY module:eastl module:corlib)

xrng_engine_add_module(${PROJECT_NAME} STATIC OPTIONS DEPENDENCY SOURCES)

## For Visual Studio
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER ${XR_PROJECT_PREFIX})