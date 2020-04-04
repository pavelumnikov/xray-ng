cmake_minimum_required(VERSION 3.10)

##

function (xrng_engine_add_renderdoc ModuleName)
	## Predefine path to RenderDoc
	set(RENDERDOC_INCLUDE_PATH ${XR_SOURCES}/sdk/renderdoc/include CACHE INTERNAL "RenderDoc path")
	## Add private include paths for selected project name
	target_include_directories(${ModuleName} PRIVATE ${RENDERDOC_INCLUDE_PATH})
endfunction(xrng_engine_add_renderdoc ModuleName)