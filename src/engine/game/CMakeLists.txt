cmake_minimum_required(VERSION 3.10)

## Add modules function
include(${CMAKE_MODULE_PATH}/add_module.cmake)
include(${CMAKE_MODULE_PATH}/install_dll_if_exists.cmake)

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
	"sources/main/main.cpp"
	"sources/main/application.cpp"
	"sources/main/application.h"
)

source_group("sources\\main" FILES ${GAME_MODULE_MAIN_SOURCES})

##

set(GAME_MODULE_MAIN_WIN_SOURCES
	"sources/main/win/splash_screen.h"
	"sources/main/win/splash_screen.cpp"
	"sources/main/win/win_main.h"
	"sources/main/win/win_main.cpp"
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

set(GAME_MODULE_TASK_SOURCES
	"sources/tasks/init_task.cpp"
	"sources/tasks/init_task.h"
	"sources/tasks/tick_task.cpp"
	"sources/tasks/tick_task.h"
)

source_group("sources\\task" FILES ${GAME_MODULE_TASK_SOURCES})

##

set(SOURCES
	${GAME_MODULE_SOURCES}
	${GAME_MODULE_MAIN_SOURCES}
	${GAME_MODULE_MODULES_SOURCES}
	${GAME_MODULE_TASK_SOURCES}
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
	module:imgui
	module:corlib
	module:engine
	win:delayimp
)

xrng_engine_add_module(${PROJECT_NAME} EXECUTABLE OPTIONS DEPENDENCY SOURCES)

## For Visual Studio
set_property(TARGET ${PROJECT_NAME} PROPERTY FOLDER ${XR_PROJECT_PREFIX})
set_property(TARGET ${PROJECT_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${GAME_PROJECT_OUTPUT}")

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION ${GAME_PROJECT_OUTPUT})
install(FILES ${PARANOIA_MODULE_RESOURCES} DESTINATION ${GAME_PROJECT_OUTPUT})

xrng_install_dll_if_exists(${FMODSDK_LL_PATH}/lib "fmod64" ${GAME_PROJECT_OUTPUT})
xrng_install_dll_if_exists(${FMODSDK_LL_PATH}/lib "fmodL64" ${GAME_PROJECT_OUTPUT})
xrng_install_dll_if_exists(${FMODSDK_STUDIO_PATH}/lib "fmodstudio64" ${GAME_PROJECT_OUTPUT})
xrng_install_dll_if_exists(${FMODSDK_STUDIO_PATH}/lib "fmodstudioL64" ${GAME_PROJECT_OUTPUT})