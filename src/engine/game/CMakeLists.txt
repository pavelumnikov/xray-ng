cmake_minimum_required(VERSION 3.10)

## Add modules function
include(${CMAKE_MODULE_PATH}/add_module.cmake)

include(CheckFunctionExists)
include(CheckIncludeFiles)
include(CMakeDependentOption)

project(${GAME_PROJECT})

##

set(GAME_MODULE_RESOURCES
	"LauncherSplash.bmp")

source_group("\\" FILES ${PARANOIA_MODULE_RESOURCES})

##

set(GAME_MODULE_MAIN_SOURCES
	"sources/main/glfw_main.cpp"
	"sources/main/glfw_application.cpp"
	"sources/main/glfw_application.h")

source_group("main" FILES ${GAME_MODULE_MAIN_SOURCES})

##

set(GAME_MODULE_MAIN_WIN_SOURCES
	"sources/main/win/splash_screen.h"
	"sources/main/win/splash_screen.cpp")

source_group("main\\win" FILES ${GAME_MODULE_MAIN_WIN_SOURCES})

##

set(GAME_MODULE_MAIN_WIN_RESOURCES
	"sources/main/win/paranoia.rc"
	"sources/main/win/resource.h"
	"sources/main/win/icon.ico")

source_group("main\\win" FILES ${GAME_MODULE_MAIN_WIN_RESOURCES})

##

set(SOURCES
	${GAME_MODULE_MAIN_SOURCES}
	${GAME_MODULE_MAIN_WIN_SOURCES}
	${GAME_MODULE_MAIN_WIN_RESOURCES}
	${GAME_MODULE_RESOURCES})

set(OPTIONS generic:cpp17=yes generic:noexceptions=yes)
set(DEPENDENCY module:eastl module:glfw module:corlib module:async_io win:delayimp)
xrng_engine_add_module(${PROJECT_NAME} EXECUTABLE OPTIONS DEPENDENCY SOURCES)

target_include_directories(${PROJECT_NAME} PRIVATE ${eastl_SOURCE_DIR}/include)

## For Visual Studio
set_property(TARGET ${PROJECT_NAME} PROPERTY FOLDER ${XR_PROJECT_PREFIX})
set_property(TARGET ${PROJECT_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${GAME_PROJECT_OUTPUT}")

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION ${GAME_PROJECT_OUTPUT})
install(FILES ${PARANOIA_MODULE_RESOURCES} DESTINATION ${GAME_PROJECT_OUTPUT})