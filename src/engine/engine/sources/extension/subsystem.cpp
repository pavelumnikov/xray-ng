// This file is a part of xray-ng engine
//

#include "pch.h"
#include "engine/extension/subsystem.h"

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, engine, extension)

//-----------------------------------------------------------------------------------------------------------
/**
 */
bool subsystem::initialize_sync_before_async()
{
    return true;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
bool subsystem::initialize_async(tasks::execution_context& ctx)
{
    XR_UNREFERENCED_PARAMETER(ctx);
    return true;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
void subsystem::tick_async(tasks::execution_context& ctx, sys::tick delta)
{
    XR_UNREFERENCED_PARAMETER(ctx, delta);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
subsystem::subsystem(context* ctx)
    : m_context(ctx)
{}

XR_NAMESPACE_END(xr, engine, extension)
//-----------------------------------------------------------------------------------------------------------