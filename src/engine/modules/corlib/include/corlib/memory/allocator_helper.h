// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/utils/type_traits.h"

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, memory)

//-----------------------------------------------------------------------------------------------------------
#if XR_COMPILER_SUPPORTS_CPP11
template<typename T, typename ... Args>
void call_emplace_construct(T* p, Args&&... args)
{
    (void)new(reinterpret_cast<void*>(p)) T(args...);
}
#endif // #if XR_COMPILER_SUPPORTS_CPP11

//-----------------------------------------------------------------------------------------------------------
template<typename T>
void call_destruct(T* p)
{
    XR_DEBUG_ASSERTION(p != nullptr);
    p->~T();
}

XR_NAMESPACE_END(xr, memory)
//-----------------------------------------------------------------------------------------------------------
