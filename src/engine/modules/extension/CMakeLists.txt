cmake_minimum_required(VERSION 3.10)

## Add modules function
include(${CMAKE_MODULE_PATH}/add_module.cmake)

include(CheckFunctionExists)
include(CheckIncludeFiles)
include(CMakeDependentOption)

project(extension)

##

set(EXTENSION_MODULE_HEADERS
	"include/extension/context.h"
	"include/extension/subsystem.h"
)

source_group("include" FILES ${EXTENSION_MODULE_HEADERS})

##

set(EXTENSION_MODULE_SOURCES
	"sources/context.cpp"
	"sources/subsystem.cpp"
)

source_group("sources" FILES ${EXTENSION_MODULE_SOURCES})

##

set(EXTENSION_MODULE_TASKS_SOURCES
	"sources/task/parallel_init_task.cpp"
	"sources/task/parallel_init_task.h"
	"sources/task/tick_task.cpp"
	"sources/task/tick_task.h"
)

source_group("sources\\task" FILES ${EXTENSION_MODULE_TASKS_SOURCES})

##

set(SOURCES
	${EXTENSION_MODULE_HEADERS}
	${EXTENSION_MODULE_SOURCES}
	${EXTENSION_MODULE_TASKS_SOURCES}
)

##

set(OPTIONS generic:cpp17=yes generic:noexceptions=yes)
set(DEPENDENCY module:eastl module:corlib)

xrng_engine_add_module(${PROJECT_NAME} STATIC OPTIONS DEPENDENCY SOURCES)

## For Visual Studio
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER ${XR_PROJECT_PREFIX})