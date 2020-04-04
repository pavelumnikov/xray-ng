cmake_minimum_required(VERSION 3.0)

## Add modules function
include(${CMAKE_MODULE_PATH}/add_module.cmake)

include(CheckFunctionExists)
include(CheckIncludeFiles)
include(CMakeDependentOption)

project(lua)

set(LUA_PUBLIC_HEADERS 
	"src/lua/lua.h"
	"src/lua/lualib.h"
	"src/lua/lauxlib.h"
	"src/lua/lua.hpp"
)

set(LUA_PRIVATE_SOURCES 
	"src/lua/lapi.c"
    "src/lua/lapi.h"
    "src/lua/lauxlib.c"
    "src/lua/lbaselib.c"
    "src/lua/lcode.c"
    "src/lua/lcode.h"
    "src/lua/lcorolib.c"
    "src/lua/lctype.c"
    "src/lua/lctype.h"
    "src/lua/ldblib.c"
    "src/lua/ldebug.c"
    "src/lua/ldebug.h"
    "src/lua/ldo.c"
    "src/lua/ldo.h"
    "src/lua/ldump.c"
    "src/lua/lfunc.c"
    "src/lua/lfunc.h"
    "src/lua/lgc.c"
    "src/lua/lgc.h"
    "src/lua/linit.c"
    "src/lua/liolib.c"
    "src/lua/ljumptab.h"
    "src/lua/llex.c"
    "src/lua/llex.h"
    "src/lua/llimits.h"
    "src/lua/lmathlib.c"
    "src/lua/lmem.c"
    "src/lua/lmem.h"
    "src/lua/loadlib.c"
    "src/lua/lobject.c"
    "src/lua/lobject.h"
    "src/lua/lopcodes.c"
    "src/lua/lopcodes.h"
    "src/lua/lopnames.h"
    "src/lua/loslib.c"
    "src/lua/lparser.c"
    "src/lua/lparser.h"
    "src/lua/lprefix.h"
    "src/lua/lstate.c"
    "src/lua/lstate.h"
    "src/lua/lstring.c"
    "src/lua/lstring.h"
    "src/lua/lstrlib.c"
    "src/lua/ltable.c"
    "src/lua/ltable.h"
    "src/lua/ltablib.c"
    "src/lua/ltm.c"
    "src/lua/ltm.h"
    "src/lua/lua.c"
    "src/lua/luaconf.h"
    "src/lua/lundump.c"
    "src/lua/lundump.h"
    "src/lua/lutf8lib.c"
    "src/lua/lvm.c"
    "src/lua/lvm.h"
    "src/lua/lzio.c"
    "src/lua/lzio.h"
)
	
source_group("include" FILES ${LUA_PUBLIC_HEADERS})
source_group("sources" FILES ${LUA_PRIVATE_SOURCES})

set(SOURCES ${LUA_PUBLIC_HEADERS} ${LUA_PRIVATE_SOURCES})
set(OPTIONS generic:purec=yes)

xrng_engine_add_thirdparty(${PROJECT_NAME} STATIC OPTIONS NoDependency SOURCES)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/lua)

## For Visual Studio
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER ${XR_PROJECT_PREFIX})