cmake_minimum_required(VERSION 3.10)

## For Visual Studio
set(XR_PROJECT_PREFIX ${XR_PROJECT_PREFIX}/engine)

add_subdirectory(modules)
add_subdirectory(game)
add_subdirectory(tools)