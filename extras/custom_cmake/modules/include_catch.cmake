cmake_minimum_required(VERSION 3.10)

##

function (xrng_engine_add_doctest ModuleName)
	## Predefine path to Catch
	set(CATCH_INCLUDE_PATH ${XR_SOURCES}/sdk/catch/include CACHE INTERNAL "catch path")
	## Add private include paths for selected project name
	target_include_directories(${ModuleName} PRIVATE ${CATCH_INCLUDE_PATH})
endfunction(xrng_engine_add_doctest ModuleName)