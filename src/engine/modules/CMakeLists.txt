cmake_minimum_required(VERSION 3.10)

## For Visual Studio
set(XR_PROJECT_PREFIX ${XR_PROJECT_PREFIX}/modules)

add_subdirectory(extension)
add_subdirectory(async_io)
add_subdirectory(gfx)
add_subdirectory(sfx)