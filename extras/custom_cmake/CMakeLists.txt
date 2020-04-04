cmake_minimum_required(VERSION 3.10)

set(PROJECT_NAME custom_cmake)

set(CMAKE_FILES 
	"add_module.cmake")

set(CMAKE_PROGRAMS_FILES 
	"programs/use_nuget.cmake")

set(CMAKE_MODULE_FILES
	"modules/include_catch.cmake"
	"modules/include_renderdoc.cmake")

source_group("\\" FILES ${CMAKE_FILES})
source_group("programs" FILES ${CMAKE_PROGRAMS_FILES})
source_group("modules" FILES ${CMAKE_MODULE_FILES})
source_group("toolchain" FILES ${CMAKE_TOOLCHAIN_FILES})

set(CMAKE_SOURCES ${CMAKE_FILES} ${CMAKE_MODULE_FILES} ${CMAKE_PROGRAMS_FILES})
add_custom_target(${PROJECT_NAME} SOURCES ${CMAKE_SOURCES})

## For Visual Studio
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER ${XR_PROJECT_PREFIX})