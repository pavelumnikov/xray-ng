cmake_minimum_required(VERSION 3.10)

## Add modules function
include(${CMAKE_MODULE_PATH}/add_module.cmake)

include(CheckFunctionExists)
include(CheckIncludeFiles)
include(CMakeDependentOption)

project(io)

##

set(IO_MODULE_HEADERS
	"include/io/api.h"
	"include/io/binary_stream_reader.h"
	"include/io/binary_stream_writer.h"
	"include/io/file_stream.h"
	"include/io/linkage.h"
	"include/io/memory_stream.h"
	"include/io/stream.h"
	"include/io/stream_reader.h"
	"include/io/stream_writer.h"
)
	
source_group("include" FILES ${IO_MODULE_HEADERS})

##

set(IO_MODULE_SOURCES
	"sources/api_win32.cpp"
	"sources/async_buffered_file_handler_win32.cpp"
	"sources/async_buffered_file_handler_win32.h"
	"sources/async_file_handler_win32.cpp"
	"sources/async_file_handler_win32.h"
	"sources/base_file_handle.cpp"
	"sources/base_file_handle.h"
	"sources/binary_stream_reader.cpp"
	"sources/binary_stream_writer.cpp"
	"sources/file_stream.cpp"
	"sources/memory_stream.cpp"
	"sources/pch.cpp"
	"sources/pch.h"
	"sources/stream.cpp"
	"sources/stream_reader.cpp"
	"sources/stream_writer.cpp"
)

source_group("sources" FILES ${IO_MODULE_SOURCES})

##

set(SOURCES ${IO_MODULE_HEADERS} ${IO_MODULE_SOURCES})

##

set(OPTIONS generic:cpp17=yes generic:noexceptions=yes)
set(DEPENDENCY module:eastl module:corlib)

xrng_engine_add_module(${PROJECT_NAME} STATIC OPTIONS DEPENDENCY SOURCES)

## For Visual Studio
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER ${XR_PROJECT_PREFIX})