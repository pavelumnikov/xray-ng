// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/types.h"

namespace xr
{

template<typename F>
class scoped_exit
{
public:
    explicit scoped_exit(F&& fn) : m_fn(fn)
    {}

    ~scoped_exit()
    {
        m_fn();
    }

    scoped_exit(scoped_exit&& other) : m_fn(std::move(other.m_fn))
    {}

    XR_DECLARE_DELETE_COPY_ASSIGNMENT(scoped_exit);

private:
    F m_fn;
};

} // namespace xr