// This file is a part of xray-ng engine
//

#include "extension/subsystem.h"

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, extension)

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
void subsystem::tick_async(tasks::execution_context& ctx, float delta)
{
    XR_UNREFERENCED_PARAMETER(ctx, delta);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
subsystem::subsystem(context* ctx)
    : m_context(ctx)
{}

XR_NAMESPACE_END(xr, extension)
//-----------------------------------------------------------------------------------------------------------