// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/types.h"

//-----------------------------------------------------------------------------------------------------------
namespace xr::utils
{

//-----------------------------------------------------------------------------------------------------------
// Simple wrapper to work with raw memory as an array. Includes array bounds checking.
template<typename T>
class array_view
{
public:
    using value_type = T;
    using pointer = T*;
    using const_pointer = T const*;
    using reference = T&;
    using const_reference = T const&;
    using size_type = size_t;

    array_view();
    array_view(pvoid memory_chunk, size_type instances_count);
    ~array_view() = default;

    XR_DECLARE_DELETE_COPY_ASSIGNMENT(array_view);
    XR_DECLARE_DEFAULT_MOVE_ASSIGNMENT(array_view);

    const_reference operator[](size_type i) const;
    reference operator[](size_type i);

    size_type size() const;
    bool is_empty() const;

    pointer get_raw_data();
    const_pointer get_raw_data() const;
    
private:
    pointer m_data;
    size_type m_count;
}; // class array_view<T>

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<typename T>
inline array_view<T>::array_view()
    : m_data { nullptr }
    , m_count { 0 }
{}

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<typename T>
inline array_view<T>::array_view(pvoid memory_chunk, size_type instances_count)
    : m_data { reinterpret_cast<T*>(memory_chunk) }
    , m_count { instances_count }
{
    XR_DEBUG_ASSERTION_MSG(m_count == 0 || m_data, "Invalid data array");
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<typename T>
inline typename array_view<T>::const_reference array_view<T>::operator[](size_type i) const
{
    XR_DEBUG_ASSERTION_MSG(i < size(), "bad index");
    return m_data[i];
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<typename T>
inline typename array_view<T>::reference array_view<T>::operator[](size_type i)
{
    XR_DEBUG_ASSERTION_MSG(i < size(), "bad index");
    return m_data[i];
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<typename T>
inline typename array_view<T>::size_type array_view<T>::size() const
{
    return m_count;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<typename T>
inline bool array_view<T>::is_empty() const
{
    return m_count == 0;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<typename T>
inline typename array_view<T>::pointer array_view<T>::get_raw_data()
{
    return m_data;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<typename T>
inline typename array_view<T>::const_pointer array_view<T>::get_raw_data() const
{
    return m_data;
}

} // namespace xr::utils
//-----------------------------------------------------------------------------------------------------------