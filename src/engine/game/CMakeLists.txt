cmake_minimum_required(VERSION 3.10)

## Add modules function
include(${CMAKE_MODULE_PATH}/add_module.cmake)
include(${CMAKE_MODULE_PATH}/modules/include_vulkan.cmake)

include(CheckFunctionExists)
include(CheckIncludeFiles)
include(CMakeDependentOption)

project(${GAME_PROJECT})

##

set(GAME_MODULE_RESOURCES
	"LauncherSplash.bmp"
)

source_group("\\" FILES ${PARANOIA_MODULE_RESOURCES})

##

set(GAME_MODULE_SOURCES
	"sources/config.cpp"
	"sources/config.h"
	"sources/constants.h"
)

source_group("sources" FILES ${GAME_MODULE_SOURCES})

##

set(GAME_MODULE_MAIN_SOURCES
	"sources/main/glfw_main.cpp"
	"sources/main/glfw_application.cpp"
	"sources/main/glfw_application.h"
)

source_group("sources\\main" FILES ${GAME_MODULE_MAIN_SOURCES})

##

set(GAME_MODULE_MAIN_WIN_SOURCES
	"sources/main/win/splash_screen.h"
	"sources/main/win/splash_screen.cpp"
)

source_group("sources\\main\\win" FILES ${GAME_MODULE_MAIN_WIN_SOURCES})

##

set(GAME_MODULE_MAIN_WIN_RESOURCES
	"sources/main/win/paranoia.rc"
	"sources/main/win/resource.h"
	"sources/main/win/icon.ico"
)

source_group("sources\\main\\win" FILES ${GAME_MODULE_MAIN_WIN_RESOURCES})

##

set(GAME_MODULE_MODULES_SOURCES
	"sources/modules/rendering_module.cpp"
	"sources/modules/rendering_module.h"
)

source_group("sources\\modules" FILES ${GAME_MODULE_MODULES_SOURCES})

##

set(GAME_MODULE_TASK_SOURCES
	"sources/tasks/rendering_subsystem_task.cpp"
	"sources/tasks/rendering_subsystem_task.h"
)

source_group("sources\\task" FILES ${GAME_MODULE_TASK_SOURCES})

##

set(GAME_MODULE_TASK_RENDERING_SOURCES)
source_group("sources\\task\\rendering" FILES ${GAME_MODULE_TASK_RENDERING_SOURCES})

##

set(SOURCES
	${GAME_MODULE_SOURCES}
	${GAME_MODULE_MAIN_SOURCES}
	${GAME_MODULE_MODULES_SOURCES}
	${GAME_MODULE_TASK_SOURCES}
	${GAME_MODULE_TASK_RENDERING_SOURCES}
)

if(WIN32)
	list(APPEND SOURCES ${GAME_MODULE_MAIN_WIN_SOURCES})
	list(APPEND SOURCES ${GAME_MODULE_MAIN_WIN_RESOURCES})
	list(APPEND SOURCES ${GAME_MODULE_RESOURCES})
endif(WIN32)

##

set(OPTIONS generic:cpp17=yes generic:noexceptions=yes)

set(DEPENDENCY
	module:eastl
	module:glfw
	module:imgui
	module:corlib
	module:async_io
	module:rendering_api
	win:delayimp
)

xrng_engine_add_module(${PROJECT_NAME} EXECUTABLE OPTIONS DEPENDENCY SOURCES)
xrng_engine_add_vulkan(${PROJECT_NAME})

## For Visual Studio
set_property(TARGET ${PROJECT_NAME} PROPERTY FOLDER ${XR_PROJECT_PREFIX})
set_property(TARGET ${PROJECT_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${GAME_PROJECT_OUTPUT}")

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION ${GAME_PROJECT_OUTPUT})
install(FILES ${PARANOIA_MODULE_RESOURCES} DESTINATION ${GAME_PROJECT_OUTPUT})