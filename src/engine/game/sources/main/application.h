// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/memory/memory_allocator_base.h"

//-----------------------------------------------------------------------------------------------------------
namespace xr::game::main
{

//-----------------------------------------------------------------------------------------------------------
struct initialize_application_desc
{
    memory::base_allocator& misc_allocator;
    memory::base_allocator& io_system_allocator;
    memory::base_allocator& gfx_allocator;
    memory::base_allocator& sfx_allocator;
};

//-----------------------------------------------------------------------------------------------------------
/**
*/
void early_initialize_application(memory::base_allocator& misc_allocator);

//-----------------------------------------------------------------------------------------------------------
/**
*/
void initialize_application(initialize_application_desc const& desc);

//-----------------------------------------------------------------------------------------------------------
/**
*/
void shutdown_application();

//-----------------------------------------------------------------------------------------------------------
/**
*/
bool run_application();

} // namespace xr::game::main
//-----------------------------------------------------------------------------------------------------------
