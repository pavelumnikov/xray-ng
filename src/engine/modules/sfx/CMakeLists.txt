cmake_minimum_required(VERSION 3.10)

## Add modules function
include(${CMAKE_MODULE_PATH}/add_module.cmake)
include(${CMAKE_MODULE_PATH}/modules/include_fmod.cmake)

include(CheckFunctionExists)
include(CheckIncludeFiles)
include(CMakeDependentOption)

project(sfx)

##

set(SFX_MODULE_HEADERS
	"include/sfx/sfx_api.h"
	"include/sfx/sfx_environment.h"
	"include/sfx/sfx_event.h"
	"include/sfx/sfx_subsystem.h"
)
	
source_group("include" FILES ${SFX_MODULE_HEADERS})

##

set(SFX_MODULE_SOURCES
	"sources/pch.cpp"
	"sources/pch.h"
	"sources/sfx_init_task.cpp"
	"sources/sfx_init_task.h"
	"sources/sfx_subsystem.cpp"
)

source_group("sources" FILES ${SFX_MODULE_SOURCES})

##

set(SFX_MODULE_FMOD_SOURCES
	"sources/fmod/fmod_device.h"
	"sources/fmod/fmod_device.cpp"
	"sources/fmod/fmod_event_descriptions.h"
	"sources/fmod/fmod_event_descriptions.cpp"
	"sources/fmod/fmod_sfx_api.h"
	"sources/fmod/fmod_sfx_api.cpp"
	"sources/fmod/fmod_sfx_environment.h"
	"sources/fmod/fmod_sfx_environment.cpp"
	"sources/fmod/fmod_sfx_event.h"
	"sources/fmod/fmod_sfx_event.cpp"
)

source_group("sources\\FMOD" FILES ${SFX_MODULE_FMOD_SOURCES})

##

set(SOURCES
	${SFX_MODULE_HEADERS}
	${SFX_MODULE_SOURCES}
	${SFX_MODULE_FMOD_SOURCES}
)

##

set(OPTIONS generic:cpp17=yes generic:noexceptions=yes)
set(DEPENDENCY module:eastl module:corlib module:extension)

xrng_engine_add_module(${PROJECT_NAME} STATIC OPTIONS DEPENDENCY SOURCES)
xrng_engine_add_fmod(${PROJECT_NAME})
## For Visual Studio
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER ${XR_PROJECT_PREFIX})