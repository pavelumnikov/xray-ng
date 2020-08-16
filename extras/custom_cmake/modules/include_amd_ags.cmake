cmake_minimum_required(VERSION 3.10)

##

function (xrng_engine_add_amd_ags ModuleName)
	## Predefine path to AMD AGS
	set(AMD_AGS_PATH ${XR_SOURCES}/sdk/agssdk/ags_lib CACHE INTERNAL "AMD AGS path")
	set(AMD_AGS_INCLUDE_PATH "${AMD_AGS_PATH}/inc" CACHE INTERNAL "AMD AGS include path")
	set(AMD_AGS_LIBRARY_PATH "${AMD_AGS_PATH}/lib" CACHE INTERNAL "AMD AGS library path")
	## Add private include paths for selected project name
	target_include_directories(${ModuleName} PRIVATE ${AMD_AGS_INCLUDE_PATH})
	target_link_directories(${ModuleName} PRIVATE ${AMD_AGS_LIBRARY_PATH})
	target_link_libraries(${ModuleName} vulkan-1)
endfunction(xrng_engine_add_vulkan ModuleName)