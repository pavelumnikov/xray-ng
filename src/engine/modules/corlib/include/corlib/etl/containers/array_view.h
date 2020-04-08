// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/types.h"

namespace xr::etl::containers
{

/// Simple wrapper to work with raw memory as an array. Includes array bounds checking.
template<class T>
class array_view
{
    T* m_data;
    size_t m_count;

public:

    array_view()
    {
        m_data = nullptr;
        m_count = 0;
    }

    array_view(void* memoryChunk, size_t instanceCount)
        : m_data((T*)memoryChunk)
        , m_count(instanceCount)
    {
        XR_DEBUG_ASSERTION_MSG(m_count == 0 || m_data, "Invalid data array");
    }

    ~array_view()
    {
        m_data = nullptr;
        m_count = 0;
    }

    const T& operator[](size_t i) const
    {
        XR_DEBUG_ASSERTION_MSG(i < size(), "bad index");
        return m_data[i];
    }

    T& operator[](size_t i)
    {
        XR_DEBUG_ASSERTION_MSG(i < size(), "bad index");
        return m_data[i];
    }

    size_t size() const
    {
        return m_count;
    }

    bool is_empty() const
    {
        return m_count == 0;
    }

    T* get_raw_data()
    {
        return m_data;
    }

    const T* get_raw_data() const
    {
        return m_data;
    }
};

} // namespace xr::etl::containers
