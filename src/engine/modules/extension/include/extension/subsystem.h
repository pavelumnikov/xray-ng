// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/tasks/task_system.h"
#include "corlib/utils/ptr.h"

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, extension)

// forward declarations
class context;

//-----------------------------------------------------------------------------------------------------------
class XR_NON_VIRTUAL subsystem : public eastl::enable_shared_from_this<subsystem>
{
public:
    XR_DECLARE_DEFAULT_DESTRUCTOR(subsystem);
    virtual bool initialize_sync_before_async();
    virtual bool initialize_async(tasks::execution_context& ctx);
    virtual void tick_async(tasks::execution_context& ctx, float delta);

    template<typename T>
    utils::shared_ptr<T> query_shared_ptr();

protected:
    subsystem(context* ctx);

private:
    context* m_context;
}; // class subsystem

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<typename T>
inline utils::shared_ptr<T> subsystem::query_shared_ptr()
{
    return eastl::static_pointer_cast<T>(shared_from_this());
}

XR_NAMESPACE_END(xr, extension)
//-----------------------------------------------------------------------------------------------------------