cmake_minimum_required(VERSION 3.10)

##

function (xrng_use_nuget_restore ModuleName)
	add_custom_command(TARGET ${ModuleName} PRE_BUILD 
		COMMAND "${CMAKE_MODULE_PATH}/programs/nuget/nuget.exe"
		restore ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.sln)
endfunction (xrng_use_nuget_restore ModuleName)