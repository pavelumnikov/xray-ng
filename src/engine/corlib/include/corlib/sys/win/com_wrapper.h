// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/sys/win/min_windows.h"

XR_NAMESPACE_BEGIN(xr, sys, win)

template< typename Type >
inline void com_safe_release(Type*& object)
{
    if(object)
    {
        object->Release();
        object = nullptr;
    }
}

template< typename Type >
union com_object_and_raw_ptr
{
    Type* com_object;
    void* raw_ptr;

    operator Type* ();
    operator void* ();
};

template< typename Type >
com_object_and_raw_ptr<Type>::operator Type* ()
{
    return com_object;
}

template< typename Type >
com_object_and_raw_ptr<Type>::operator void* ()
{
    return raw_ptr;
}

XR_NAMESPACE_END(xr, sys, win)