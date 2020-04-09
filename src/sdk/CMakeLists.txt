cmake_minimum_required(VERSION 3.10)

## For Visual Studio
set(XR_PROJECT_PREFIX ${XR_PROJECT_PREFIX}/sdk)

add_subdirectory(eastl)
add_subdirectory(glfw)
add_subdirectory(imgui)
add_subdirectory(lua)