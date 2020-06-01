// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/sys/win/min_windows.h"
#include "corlib/memory/memory_allocator_base.h"

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, game, win)

//-----------------------------------------------------------------------------------------------------------
/**
 */
bool create_main_window(memory::base_allocator& allocator, uint32_t width, uint32_t height, bool fullscreen);

//-----------------------------------------------------------------------------------------------------------
/**
 */
void show_main_window();

//-----------------------------------------------------------------------------------------------------------
/**
 */
bool run_main_loop();

//-----------------------------------------------------------------------------------------------------------
/**
 */
void destroy_main_window();

XR_NAMESPACE_END(xr, game, win)
//-----------------------------------------------------------------------------------------------------------