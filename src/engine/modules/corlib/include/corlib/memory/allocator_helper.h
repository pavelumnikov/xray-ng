// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/etl/type_traits.h"

//------------------------------------------------------------------------------
namespace xr::memory
{

//------------------------------------------------------------------------------
template<typename T, typename ... Args>
void call_emplace_construct(T* p, Args&&... args)
{
    (void)new(reinterpret_cast<void*>(p)) T(args...);
}

//------------------------------------------------------------------------------
template<typename T>
void call_destruct(T* p)
{
    XR_DEBUG_ASSERTION(p != nullptr);
    p->~T();
}

} // namespace xr::etl
//------------------------------------------------------------------------------