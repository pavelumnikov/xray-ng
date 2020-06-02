cmake_minimum_required(VERSION 3.10)

##

function (xrng_install_dll_if_exists BasePath DllName TargetPath)
	set(SOURCE_DLL_MODULE "${BasePath}/${DllName}.dll" CACHE INTERNAL "DLL file to install")
	set(SOURCE_PDB_MODULE "${BasePath}/${DllName}.pdb" CACHE INTERNAL "PDB file to install")
	if(EXISTS ${SOURCE_DLL_MODULE})
		install (FILES ${SOURCE_DLL_MODULE} DESTINATION ${TargetPath})
		if(EXISTS ${SOURCE_PDB_MODULE})
			install (FILES ${SOURCE_PDB_MODULE} DESTINATION ${TargetPath})
		endif(EXISTS ${SOURCE_PDB_MODULE})
	endif(EXISTS ${SOURCE_DLL_MODULE})
endfunction(xrng_install_dll_if_exists BasePath DllName TargetPath)