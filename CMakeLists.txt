cmake_minimum_required(VERSION 3.10)
cmake_policy(SET CMP0054 NEW)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

## For Visual Studio
set(XR_PROJECT_PREFIX xray-ng)

set(GAME_PROJECT clearsky)
set(GAME_PROJECT_OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/result)
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/extras/custom_cmake)

project(xrengine-ng)

set(XR_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src)

if(${WITH_TESTS})
	enable_testing()
endif(${WITH_TESTS})

add_subdirectory(extras)
add_subdirectory(src)