cmake_minimum_required(VERSION 3.10)

##

function (xrng_engine_add_fmod ModuleName)
	## Predefine path to FMOD
	set(FMODSDK_PATH $ENV{FMODSDK_DIR} CACHE INTERNAL "FMOD SDK path")
	set(FMODSDK_LL_PATH ${FMODSDK_PATH}/api/lowlevel CACHE INTERNAL "FMOD Low-Level library path")
	set(FMODSDK_STUDIO_PATH ${FMODSDK_PATH}/api/studio CACHE INTERNAL "FMOD Studio library path")
	
	## Add private include paths for selected project name
	target_include_directories(${ModuleName} PRIVATE ${FMODSDK_LL_PATH}/inc ${FMODSDK_STUDIO_PATH}/inc)
	target_link_directories(${ModuleName} PUBLIC ${FMODSDK_LL_PATH}/lib ${FMODSDK_STUDIO_PATH}/lib)
	target_link_libraries(${ModuleName} fmod64_vc fmodstudio64_vc)
endfunction(xrng_engine_add_fmod ModuleName)