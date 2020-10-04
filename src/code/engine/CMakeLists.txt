cmake_minimum_required(VERSION 3.10)

## Add modules function
include(${CMAKE_MODULE_PATH}/add_module.cmake)

include(CheckFunctionExists)
include(CheckIncludeFiles)
include(CMakeDependentOption)

project(engine)

##

set(ENGINE_MODULE_HEADERS
	"include/engine/linkage.h"
)

source_group("include\\engine" FILES ${ENGINE_MODULE_HEADERS})

##

set(ENGINE_MODULE_SOURCES
	"sources/pch.h"
	"sources/pch.cpp"
)

source_group("sources" FILES ${ENGINE_MODULE_SOURCES})

##

set(ENGINE_MODULE_IO_HEADERS
	"include/engine/io/api.h"
	"include/engine/io/binary_stream_reader.h"
	"include/engine/io/binary_stream_writer.h"
	"include/engine/io/file_stream.h"
	"include/engine/io/memory_stream.h"
	"include/engine/io/stream.h"
	"include/engine/io/stream_reader.h"
	"include/engine/io/stream_writer.h"
)

source_group("include\\engine\\io" FILES ${ENGINE_MODULE_IO_HEADERS})

##

set(ENGINE_MODULE_IO_SOURCES
	"sources/io/base_file_handle.h"
	"sources/io/base_file_handle.cpp"
	"sources/io/binary_stream_reader.cpp"
	"sources/io/binary_stream_writer.cpp"
	"sources/io/file_stream.cpp"
	"sources/io/memory_stream.cpp"
	"sources/io/stream.cpp"
	"sources/io/stream_reader.cpp"
	"sources/io/stream_writer.cpp"
)

source_group("sources\\io" FILES ${ENGINE_MODULE_IO_SOURCES})

##

if(WIN32)
	set(ENGINE_MODULE_IO_WIN32_SOURCES
		"sources/io/api_win32.cpp"
		"sources/io/async_buffered_file_handler_win32.cpp"
		"sources/io/async_buffered_file_handler_win32.h"
		"sources/io/async_file_handler_win32.cpp"
		"sources/io/async_file_handler_win32.h"
	)
	
	source_group("sources\\io" FILES ${ENGINE_MODULE_IO_WIN32_SOURCES})
endif(WIN32)

##

set(ENGINE_MODULE_EXTENSION_HEADERS
	"include/engine/extension/context.h"
	"include/engine/extension/subsystem.h"
)

source_group("include\\engine\\extension" FILES ${ENGINE_MODULE_EXTENSION_HEADERS})

##

set(ENGINE_MODULE_EXTENSION_SOURCES
	"sources/extension/context.cpp"
	"sources/extension/subsystem.cpp"
)

source_group("sources\\extension" FILES ${ENGINE_MODULE_EXTENSION_SOURCES})

##

set(ENGINE_MODULE_EXTENSION_TASK_SOURCES
	"sources/extension/task/parallel_init_task.cpp"
	"sources/extension/task/parallel_init_task.h"
	"sources/extension/task/tick_task.cpp"
	"sources/extension/task/tick_task.h"
)

source_group("sources\\extension\\task" FILES ${ENGINE_MODULE_EXTENSION_TASK_SOURCES})

##

set(SOURCES
	${ENGINE_MODULE_HEADERS}
	${ENGINE_MODULE_SOURCES}
	${ENGINE_MODULE_IO_HEADERS}
	${ENGINE_MODULE_IO_SOURCES}
	${ENGINE_MODULE_EXTENSION_HEADERS}
	${ENGINE_MODULE_EXTENSION_SOURCES}
	${ENGINE_MODULE_EXTENSION_TASK_SOURCES}
)

##

if(WIN32)
	list(APPEND SOURCES ${ENGINE_MODULE_IO_WIN32_SOURCES})
endif(WIN32)

##

set(OPTIONS generic:cpp17=yes generic:noexceptions=yes)
set(DEPENDENCY module:eastl module:corlib)

xrng_engine_add_module(${PROJECT_NAME} STATIC OPTIONS DEPENDENCY SOURCES)
## For Visual Studio
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER ${XR_PROJECT_PREFIX})