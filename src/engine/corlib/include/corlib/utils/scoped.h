// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/types.h"

XR_NAMESPACE_BEGIN(xr, utils)

template<typename F>
class scoped_exit
{
public:
    explicit scoped_exit(F&& fn);
    scoped_exit(scoped_exit&& other);
    ~scoped_exit()

    XR_DECLARE_DELETE_COPY_ASSIGNMENT(scoped_exit);

private:
    F m_fn;
}; // class scoped_exit<F>

template<typename F>
inline scoped_exit<F>::scoped_exit(F&& fn)
    : m_fn(fn)
{}


template<typename F>
inline scoped_exit<F>::scoped_exit(scoped_exit&& other)
    : m_fn(eastl::move(other.m_fn))
{}

template<typename F>
inline scoped_exit<F>::~scoped_exit()
{
    m_fn();
}

XR_NAMESPACE_END(xr, utils)