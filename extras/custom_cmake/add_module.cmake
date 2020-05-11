cmake_minimum_required(VERSION 3.10)

##

macro(xrng_check_platform)
	if(WIN32)
		message(" - Platform: Windows")
	elseif(APPLE AND DARWIN)
		message(" - Platform: macOS")
	elseif(UNIX)
		message(" - Platform: UNIX")
	else()
		message(FATAL_ERROR "Unknown platform!")
	endif(WIN32)
endmacro(xrng_check_platform)

##

function (xrng_fixup_pch_internal ModuleName AffectedSources)
	set(PRECOMPILED_BASE_NAME pch CACHE INTERNAL "Internal representation of sources affected by PCH")
	set(XR_PCH_AFFECTED_LIST "" CACHE INTERNAL "Internal representation of sources affected by PCH")

	# These paths are always relative to CMakeLists of current module

	set(PRECOMPILED_HEADER "${PRECOMPILED_BASE_NAME}.h")
	set(PRECOMPILED_SRC "${PRECOMPILED_BASE_NAME}.cpp")

	set(PRECOMPILED_HEADER_PATH "sources/${PRECOMPILED_HEADER}")
	set(PRECOMPILED_SRC_PATH "sources/${PRECOMPILED_SRC}")

	if(NOT (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${PRECOMPILED_HEADER_PATH} OR 
	  EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${PRECOMPILED_SRC_PATH}) )
		message(" - Precompiled header: could not find PCH - skipping")
		return()

	else()
		message(" - Precompiled header: yes")

	endif(NOT (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${PRECOMPILED_HEADER_PATH} OR 
	  EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${PRECOMPILED_SRC_PATH}))

	list(APPEND XR_PCH_AFFECTED_LIST ${${AffectedSources}})
	list(FILTER XR_PCH_AFFECTED_LIST INCLUDE REGEX "(.cpp|.cxx)$")
	list(FILTER XR_PCH_AFFECTED_LIST EXCLUDE REGEX "(${PRECOMPILED_BASE_NAME}.cpp|${PRECOMPILED_BASE_NAME}.h)$")

	# Compiled binary path is always relative to pregenerated solution by the CMake
	set(MODULE_PRECOMPILED_BINARY "${CMAKE_CURRENT_BINARY_DIR}/${PRECOMPILED_BASE_NAME}.pch")

	set_source_files_properties(${XR_PCH_AFFECTED_LIST}
		PROPERTIES COMPILE_FLAGS "/Yu\"${PRECOMPILED_HEADER}\" /FI\"${PRECOMPILED_HEADER}\" /Fp\"${MODULE_PRECOMPILED_BINARY}\""
		OBJECT_DEPENDS "${MODULE_PRECOMPILED_BINARY}")
	set_source_files_properties(${PRECOMPILED_SRC_PATH}
		PROPERTIES COMPILE_FLAGS "/Yc\"${PRECOMPILED_HEADER}\" /Fp\"${MODULE_PRECOMPILED_BINARY}\""
		OBJECT_OUTPUTS "${MODULE_PRECOMPILED_BINARY}")
endfunction(xrng_fixup_pch_internal ModuleName AffectedSources)

##

function (xrng_fixup_single_option_internal ModuleName SingleOption)
	## Name of the project
	set(XR_PROJECT_NAME "${ModuleName}" CACHE INTERNAL "Project name for internal representation")
	# Strip leading spaces
	string(REGEX REPLACE "^[ ]+" "" KeyAndValue ${${SingleOption}})
	# Find variable name
	string(REGEX MATCH "^[^=]+" Name ${KeyAndValue})
	# Find the value
	string(REPLACE "${Name}=" "" Value ${KeyAndValue})

	if(${Name} STREQUAL "noexceptions")
		if(${Value} STREQUAL "yes")
			message(" - Option: not using exceptions")
			# Visual Studio - only via c++latest (sad...)
			if (MSVC)
				target_compile_options(${XR_PROJECT_NAME} PRIVATE "/EHs-c-")
				target_compile_definitions(${XR_PROJECT_NAME} PRIVATE "_HAS_EXCEPTIONS=0")
			else()
				target_compile_options(${XR_PROJECT_NAME} PRIVATE -fno-exceptions)
			endif(MSVC)
		endif(${Value} STREQUAL "yes")
	endif(${Name} STREQUAL "noexceptions")
	
	if(${Name} STREQUAL "cpp17")
		if(${Value} STREQUAL "yes")
			message(" - Option: C++17 Language Specification")
			# First of all try to enable C++17 where we able to do that.
			#target_compile_features(${XR_PROJECT_NAME} PUBLIC cxx_std_17)
			# Visual Studio - only via c++latest (sad...)
			if (MSVC_VERSION GREATER_EQUAL "1900")
				# with coroutines support in MSVC
				set_property(TARGET ${XR_PROJECT_NAME} PROPERTY COMPILE_FLAGS "/std:c++latest /await")
			# Clang - including libc++, libc++experimental and pthead
			elseif (CMAKE_CXX_COMPILER_ID MATCHES Clang)
				set_property(TARGET ${XR_PROJECT_NAME} PROPERTY COMPILE_FLAGS "-stdlib=libc++ -pthread")
				target_link_libraries(${XR_PROJECT_NAME} c++experimental pthread)
			endif(MSVC_VERSION GREATER_EQUAL "1900")
		endif(${Value} STREQUAL "yes")
	endif(${Name} STREQUAL "cpp17")
	
	if(${Name} STREQUAL "purec")
		if(${Value} STREQUAL "yes")
			message(" - Option: Pure C project")
			set_property(TARGET ${XR_PROJECT_NAME} PROPERTY LINKER_LANGUAGE C)
		endif(${Value} STREQUAL "yes")
	endif(${Name} STREQUAL "purec")

	if(${Name} STREQUAL "asm")
		if(${Value} STREQUAL "yes")
			message(" - Options: Use Assembler")
			enable_language(ASM_MASM)
		endif(${Value} STREQUAL "yes")
	endif(${Name} STREQUAL "asm")
endfunction(xrng_fixup_single_option_internal ModuleName SingleOption)

##

function (xrng_fixup_options_internal ModuleName ModuleOptions)
	## Name of the project
	set(XR_PROJECT_NAME "${ProjectName}" CACHE INTERNAL "Project name for internal representation")
	## Query all options
	set(XR_OPTIONS_LIST "" CACHE INTERNAL "Internal representation of options")
	## Enabled options
	set(XR_ACTUAL_OPTIONS_LIST "" CACHE INTERNAL "Internal representation of enabled options")
	
	## Check if NoOptions is set
	if(${ModuleOptions} STREQUAL "NoOptions")
		message(" - Additional options: no additional options - skipping")
		return()
	else()
		message(" - Additional options: yes")
		## if any option exists
		list(APPEND XR_OPTIONS_LIST ${${ModuleOptions}})
	endif(${ModuleOptions} STREQUAL "NoOptions")
	
	## Iterate through all options and add it to include list (only base modules pathes)
	foreach(ProjectOptionName IN LISTS XR_OPTIONS_LIST)
		# Strip leading spaces
		string(REGEX REPLACE "^[ ]+" "" KeyAndValue ${ProjectOptionName})
		# Find variable name
		string(REGEX MATCH "^[^:]+" Name ${KeyAndValue})
		# Find the value
		string(REPLACE "${Name}:" "" Value ${KeyAndValue})

		if(WIN32)
			if("${Name}" STREQUAL "win")
				list(APPEND XR_ACTUAL_OPTIONS_LIST "${Value}")
			endif("${Name}" STREQUAL "win")
		endif(WIN32)

		if(UNIX)
			if("${Name}" STREQUAL "unix")
				list(APPEND XR_ACTUAL_OPTIONS_LIST "${Value}")
			endif("${Name}" STREQUAL "unix")
		endif(UNIX)

		if(APPLE AND DARWIN)
			if("${Name}" STREQUAL "apple")
				list(APPEND XR_ACTUAL_OPTIONS_LIST "${Value}")
			endif("${Name}" STREQUAL "apple")
		endif(APPLE AND DARWIN)

		if("${Name}" STREQUAL "generic")
			list(APPEND XR_ACTUAL_OPTIONS_LIST "${Value}")
		endif("${Name}" STREQUAL "generic")
	endforeach(ProjectOptionName IN LISTS XR_OPTIONS_LIST)
	
	## Apply options for the project
	foreach(OptionName IN LISTS XR_ACTUAL_OPTIONS_LIST)
		xrng_fixup_single_option_internal(${XR_PROJECT_NAME} OptionName)
	endforeach(OptionName IN LISTS XR_ACTUAL_OPTIONS_LIST)
endfunction(xrng_fixup_options_internal ModuleName ModuleOptions)

##

function (xrng_fixup_dependencies_internal ModuleName ModuleDependencies)
	## Name of the project
	set(XR_PROJECT_NAME "${ModuleName}" CACHE INTERNAL "Project name for internal representation")
	## Query dependencies
	set(XR_DEPENDENCY_LIST "" CACHE INTERNAL "Internal representation of dependencies")

	set(XR_MODULE_DEPENDENCY_LIST "" CACHE INTERNAL "Internal representation of module dependencies")
	set(XR_EXTERNAL_DEPENDENCY_LIST "" CACHE INTERNAL "Internal representation of external dependencies")
	set(XR_PLATFORM_DEPENDENCY_LIST "" CACHE INTERNAL "Internal representation of platform dependencies")

	## Check if NoDependency is set
	if(${ModuleDependencies} STREQUAL "NoDependency")
		message(" - Dependencies: no dependencies - skipping")
		return()
	else()
		message(" - Dependencies: yes")
		## if any dependency exists
		list(APPEND XR_DEPENDENCY_LIST ${${ProjectDependencies}})
	endif(${ModuleDependencies} STREQUAL "NoDependency")

	## Iterate through all dependencies and add it to include list (only base modules pathes)
	foreach(ProjectDependencyName IN LISTS XR_DEPENDENCY_LIST)
		# Strip leading spaces
		string(REGEX REPLACE "^[ ]+" "" KeyAndValue ${ProjectDependencyName})
		# Find variable name
		string(REGEX MATCH "^[^:]+" Name ${KeyAndValue})
		# Find the value
		string(REPLACE "${Name}:" "" Value ${KeyAndValue})

		if(WIN32)
			if("${Name}" STREQUAL "win")
				message(" - Dependency (win): ${Value}")
				list(APPEND XR_PLATFORM_DEPENDENCY_LIST "${Value}")
			endif("${Name}" STREQUAL "win")
		endif(WIN32)

		if(UNIX)
			if("${Name}" STREQUAL "unix")
				message(" - Dependency (linux): ${Value}")
				list(APPEND XR_PLATFORM_DEPENDENCY_LIST "${Value}")
			endif("${Name}" STREQUAL "unix")
		endif(UNIX)

		if(APPLE AND DARWIN)
			if("${Name}" STREQUAL "apple")
				message(" - Dependency (apple): ${Value}")
				list(APPEND XR_PLATFORM_DEPENDENCY_LIST "${Value}")
			endif("${Name}" STREQUAL "apple")
		endif(APPLE AND DARWIN)

		if("${Name}" STREQUAL "module")
			message(" - Dependency (internal module): ${Value}")
			list(APPEND XR_MODULE_DEPENDENCY_LIST "${Value}")
		endif("${Name}" STREQUAL "module")
	endforeach(ProjectDependencyName IN LISTS XR_DEPENDENCY_LIST)

	if(XR_MODULE_DEPENDENCY_LIST)
		add_dependencies(${XR_PROJECT_NAME} ${XR_MODULE_DEPENDENCY_LIST})
		foreach(ModuleDependencyName IN LISTS XR_MODULE_DEPENDENCY_LIST)
			target_include_directories(${XR_PROJECT_NAME} PRIVATE ${${ModuleDependencyName}_SOURCE_DIR}/include)
		endforeach()
		target_link_libraries(${XR_PROJECT_NAME} ${XR_MODULE_DEPENDENCY_LIST})
	endif()

	if(XR_EXTERNAL_DEPENDENCY_LIST)
		add_dependencies(${XR_PROJECT_NAME} ${XR_EXTERNAL_DEPENDENCY_LIST})
		target_link_libraries(${XR_PROJECT_NAME} ${XR_EXTERNAL_DEPENDENCY_LIST})
	endif()
endfunction(xrng_fixup_dependencies_internal ModuleName ModuleDependencies)

##

function (xrng_fixup_definitions_internal ProjectName ProjectType)
	if(NOT ProjectName)
		message(FATAL_ERROR "ProjectName is not set!")
	endif(NOT ProjectName)

	if(NOT ProjectType)
		message(FATAL_ERROR "ProjectType is not set for ${ProjectName}!")
	endif(NOT ProjectType)

	if(NOT ((${ProjectType} STREQUAL "EXECUTABLE") OR (${ProjectType} STREQUAL "STATIC") OR 
		(${ProjectType} STREQUAL "SHARED") OR (${ProjectType} STREQUAL "UNITTEST")))

		message(FATAL_ERROR "ProjectType is wrong for ${ProjectName}! Current type: ${ProjectType}")

	endif(NOT ((${ProjectType} STREQUAL "EXECUTABLE") OR (${ProjectType} STREQUAL "STATIC") OR
		(${ProjectType} STREQUAL "SHARED") OR (${ProjectType} STREQUAL "UNITTEST")))

	## Prefix must be always empty!
	set_property(TARGET ${ProjectName} PROPERTY PREFIX "")
	set(XR_PLATFORM_NAME "" CACHE INTERNAL "Internal representation of platform name")

	if(WIN32)
		set(XR_PLATFORM_NAME win32)
	elseif(UNIX)
		set(XR_PLATFORM_NAME unix)
	elseif(APPLE AND DARWIN)
		set(XR_PLATFORM_NAME macos)
	endif(WIN32)

	if(CMAKE_SIZEOF_VOID_P EQUAL 8) # 64 bits
		target_compile_definitions(${ProjectName} PRIVATE XRAY_PLATFORM_64BIT)
	elseif(CMAKE_SIZEOF_VOID_P EQUAL 4) # 32 bits
		target_compile_definitions(${ProjectName} PRIVATE XRAY_PLATFORM_32BIT)
	endif()

	if(${ProjectType} STREQUAL "STATIC")
		set_property(TARGET ${ProjectName} PROPERTY LIBRARY_OUTPUT_NAME "${ProjectName}-static-${XR_PLATFORM_NAME}")
	elseif(${ProjectType} STREQUAL "SHARED")
		set_property(TARGET ${ProjectName} PROPERTY LIBRARY_OUTPUT_NAME "${ProjectName}-dynamic-${XR_PLATFORM_NAME}")
		set_property(TARGET ${ProjectName} PROPERTY RUNTIME_OUTPUT_NAME "${ProjectName}-dynamic-${XR_PLATFORM_NAME}")
	endif(${ProjectType} STREQUAL "STATIC")

	## Fixup definition for uniform Windows platform (includes Xbox One)
	if(WIN32)
		set_property(TARGET ${ProjectName} PROPERTY DEBUG_POSTFIX -debug)
		target_compile_options(${ProjectName} PRIVATE "/MD$<$<STREQUAL:$<CONFIGURATION>,Debug>:d>")
		## An .obj file exceedes the number of allowable sections, a COFF object file format limitation in 64-bit mode
		target_compile_options(${ProjectName} PRIVATE /bigobj)
	endif(WIN32)

	## Setup debugging definitions
	target_compile_definitions(${ProjectName} PRIVATE "$<$<STREQUAL:$<CONFIGURATION>,Debug>:XRAY_USE_DEBUG_POINTERS>")
	target_compile_definitions(${ProjectName} PRIVATE "$<$<STREQUAL:$<CONFIGURATION>,Debug>:DEBUG>")

	if(${ProjectType} STREQUAL "SHARED")
		if(UNIX)
			set_property(TARGET ${ProjectName} PROPERTY SUFFIX ".so.1")
		endif(UNIX)
	endif(${ProjectType} STREQUAL "SHARED")
	
	## Fixup definitions for platforms
	if(WIN32)
		target_compile_definitions(${ProjectName} PRIVATE XRAY_PLATFORM_WINDOWS)
	elseif(APPLE AND DARWIN)
		target_compile_definitions(${ProjectName} PRIVATE XRAY_PLATFORM_OSX)
		target_compile_definitions(${ProjectName} PRIVATE XR_MAC_OS_X_VERSION=${MAC_OS_X_VERSION})
	elseif(UNIX)
		target_compile_definitions(${ProjectName} PRIVATE XRAY_PLATFORM_LINUX)
		target_compile_definitions(${ProjectName} PRIVATE _GNU_SOURCE)
	endif(WIN32)

	## Fixup project definitions for specific platform
	if (WIN32 AND (${ProjectType} STREQUAL "EXECUTABLE"))
		set_property(TARGET ${ProjectName} PROPERTY LINK_FLAGS_DEBUG "/SUBSYSTEM:windows")
		set_property(TARGET ${ProjectName} PROPERTY LINK_FLAGS_RELWITHDEBINFO "/SUBSYSTEM:windows")
		set_property(TARGET ${ProjectName} PROPERTY LINK_FLAGS_RELEASE "/SUBSYSTEM:windows")
		set_property(TARGET ${ProjectName} PROPERTY LINK_FLAGS_MINSIZEREL "/SUBSYSTEM:windows")
	endif(WIN32 AND (${ProjectType} STREQUAL "EXECUTABLE"))

	## Always compile module with PIC (since all 3rdparty libraries are always PIC and this is good practice)
	set_target_properties(${ProjectName} PROPERTIES OUTPUT_NAME ${ProjectName} POSITION_INDEPENDENT_CODE ON)

	if(MSVC)
		if(${ProjectType} STREQUAL "EXECUTABLE")
			set_property(TARGET ${ProjectName} PROPERTY PROJECT_LABEL "${ProjectName}-exec-${XR_PLATFORM_NAME}")
		elseif(${ProjectType} STREQUAL "STATIC")
			set_property(TARGET ${ProjectName} PROPERTY PROJECT_LABEL "${ProjectName}-static-${XR_PLATFORM_NAME}")
		elseif(${ProjectType} STREQUAL "SHARED")
			set_property(TARGET ${ProjectName} PROPERTY PROJECT_LABEL "${ProjectName}-dll-${XR_PLATFORM_NAME}")
		endif(${ProjectType} STREQUAL "EXECUTABLE")
	endif(MSVC)

	## When we build static library or executables we need link-time optimizations
	set_property(TARGET ${ProjectName} PROPERTY INTERPROCEDURAL_OPTIMIZATION True)

	## Always include root folder of this module
	target_include_directories(${ProjectName} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

	## Predefine additionals
	if(${WITH_EDITOR})
		if(${WITH_EDITOR} STREQUAL "1")
			## Use embedded editor if we need
			target_compile_definitions(${ProjectName} PRIVATE XR_WITH_EDITOR)
		endif(${WITH_EDITOR} STREQUAL "1")
	endif(${WITH_EDITOR})
endfunction(xrng_fixup_definitions_internal ProjectName ProjectType)

##

function (xrng_engine_add_module ProjectName ProjectType ProjectOptions ProjectDependencies ProjectPrivate)
	message("------- Module: ${ProjectName} ----------")
	message(" - Type: ${ProjectType}")

	xrng_check_platform()

	if(${ProjectType} STREQUAL "EXECUTABLE")
		## Add executable as it is
		add_executable(${ProjectName} ${${ProjectPrivate}})
	else()
		## Add library as it is
		add_library(${ProjectName} ${ProjectType} ${${ProjectPrivate}})
	endif(${ProjectType} STREQUAL "EXECUTABLE")

	## Just call internal function to fullfill module
	xrng_fixup_definitions_internal(${ProjectName} ${ProjectType})
	## Check for dependencies
	xrng_fixup_dependencies_internal(${ProjectName} ${ProjectDependencies})
	## Check for options
	xrng_fixup_options_internal(${ProjectName} ${ProjectOptions})
	## Check for precompiled headers
	xrng_fixup_pch_internal(${ProjectName} ${ProjectPrivate})
endfunction(xrng_engine_add_module ProjectName ProjectType ProjectOptions ProjectDependencies ProjectPrivate)

##

function (xrng_engine_add_thirdparty ProjectName ProjectType ProjectOptions ProjectDependencies ProjectPrivate)
	message("------- Thirdparty: ${ProjectName} ----------")
	message(" - Type: ${ProjectType}")

	xrng_check_platform()

	if(${ProjectType} STREQUAL "EXECUTABLE")
		## Add executable as it is
		add_executable(${ProjectName} ${${ProjectPrivate}})
	else()
		## Add library as it is
		add_library(${ProjectName} ${ProjectType} ${${ProjectPrivate}})
	endif(${ProjectType} STREQUAL "EXECUTABLE")

	## Check for dependencies
	xrng_fixup_dependencies_internal(${ProjectName} ${ProjectDependencies})
	## Check for options
	xrng_fixup_options_internal(${ProjectName} ${ProjectOptions})
	## Always compile module with PIC (since all 3rdparty libraries are always PIC and this is good practice)
	set_target_properties(${ProjectName} PROPERTIES OUTPUT_NAME ${ProjectName} POSITION_INDEPENDENT_CODE ON)
endfunction(xrng_engine_add_thirdparty ProjectName ProjectType ProjectOptions ProjectDependencies ProjectPrivate)

##

function (xrng_engine_add_unittest ProjectName ProjectOptions ProjectDependencies ProjectPrivate)
	message("------- Unittest: ${ProjectName} ----------")

	xrng_check_platform()

	## Add executable as it is
	add_executable(${ProjectName} ${${ProjectPrivate}})
	add_test(NAME ${ProjectName}_unittest COMMAND ${ProjectName})
	## Just call internal function to fullfill module
	xrng_fixup_definitions_internal(${ProjectName} UNITTEST)
	## Check for dependencies
	xrng_fixup_dependencies_internal(${ProjectName} ${ProjectDependencies})
	## Check for options
	xrng_fixup_options_internal(${ProjectName} ${ProjectOptions})
	## Always compile module with PIC (since all 3rdparty libraries are always PIC and this is good practice)
	set_target_properties(${ProjectName} PROPERTIES OUTPUT_NAME ${ProjectName} POSITION_INDEPENDENT_CODE ON)
endfunction(xrng_engine_add_unittest ProjectName ProjectOptions ProjectDependencies ProjectPrivate)

##