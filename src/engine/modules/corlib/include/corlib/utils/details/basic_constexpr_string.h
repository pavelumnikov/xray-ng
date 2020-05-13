// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/utils/string_view.h"
#include "EASTL/type_traits.h"

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, utils, details)

//-----------------------------------------------------------------------------------------------------------
template<typename T, size_t N>
class basic_constexpr_string final
{
public:
    typedef basic_constexpr_string this_type;
    typedef T value_type;
    typedef T* pointer;
    typedef T const* const_pointer;
    typedef T& reference;
    typedef T const& const_reference;
    typedef pointer iterator;
    typedef const_pointer const_iterator;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
    typedef eastl::basic_string_view<T> view_type;

    XR_CONSTEXPR_OR_ENUM(size_type, npos, -1);

    XR_CONSTEXPR_CPP14_OR_INLINE basic_constexpr_string() XR_NOEXCEPT;

    template<size_t M>
    explicit XR_CONSTEXPR_CPP14_OR_INLINE basic_constexpr_string(value_type const(&str)[M]) XR_NOEXCEPT;

    XR_CONSTEXPR_CPP14_OR_INLINE basic_constexpr_string(view_type str) XR_NOEXCEPT;

    XR_CONSTEXPR_CPP14_OR_INLINE basic_constexpr_string(this_type const& rhs) XR_NOEXCEPT = default;
    XR_CONSTEXPR_CPP14_OR_INLINE basic_constexpr_string(this_type&& rhs) XR_NOEXCEPT = default;

    XR_CONSTEXPR_CPP14_OR_INLINE this_type& operator=(this_type const& rhs) XR_NOEXCEPT = default;
    XR_CONSTEXPR_CPP14_OR_INLINE this_type& operator=(this_type&& rhs) XR_NOEXCEPT = default;

    XR_CONSTEXPR_CPP14_OR_INLINE operator eastl::basic_string_view<T>() const XR_NOEXCEPT;

    XR_CONSTEXPR_CPP14_OR_INLINE size_type size() const XR_NOEXCEPT;

    XR_CONSTEXPR_CPP14_OR_INLINE const_iterator begin() const XR_NOEXCEPT;
    XR_CONSTEXPR_CPP14_OR_INLINE const_iterator end() const XR_NOEXCEPT;

    XR_CONSTEXPR_CPP14_OR_INLINE const_pointer c_str() const XR_NOEXCEPT;

private:
    value_type m_str[N];
}; // class basic_constexpr_string

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<typename T, size_t N>
XR_CONSTEXPR_CPP14_OR_INLINE
basic_constexpr_string<T, N>::basic_constexpr_string() XR_NOEXCEPT
    : m_str()
{}

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<typename T, size_t N>
template<size_t M>
XR_CONSTEXPR_CPP14_OR_INLINE
basic_constexpr_string<T, N>::basic_constexpr_string(value_type const(&str)[M]) XR_NOEXCEPT
{
    XR_STATIC_ASSERT(M <= N, "input string");
    for(size_t i = 0; i < M; ++i)
        m_str[i] = str[i];
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<typename T, size_t N>
XR_CONSTEXPR_CPP14_OR_INLINE basic_constexpr_string<T, N>::basic_constexpr_string(view_type str) XR_NOEXCEPT
{
    XR_DEBUG_ASSERTION_MSG(N <= str.size(), "input string");
    for(size_type i = 0; i < str.size(); ++i)
        m_str[i] = str[i];
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<typename T, size_t N>
XR_CONSTEXPR_CPP14_OR_INLINE typename basic_constexpr_string<T, N>::const_pointer
basic_constexpr_string<T, N>::c_str() const XR_NOEXCEPT
{
    return m_str;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<typename T, size_t N>
XR_CONSTEXPR_CPP14_OR_INLINE
basic_constexpr_string<T, N>::operator eastl::basic_string_view<T>() const XR_NOEXCEPT
{
    return { begin(), size() };
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<typename T, size_t N>
XR_CONSTEXPR_CPP14_OR_INLINE typename basic_constexpr_string<T, N>::size_type
basic_constexpr_string<T, N>::size() const XR_NOEXCEPT
{
    return N;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<typename T, size_t N>
XR_CONSTEXPR_CPP14_OR_INLINE typename basic_constexpr_string<T, N>::const_iterator
basic_constexpr_string<T, N>::begin() const XR_NOEXCEPT
{
    return m_str;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<typename T, size_t N>
XR_CONSTEXPR_CPP14_OR_INLINE typename basic_constexpr_string<T, N>::const_iterator
basic_constexpr_string<T, N>::end() const XR_NOEXCEPT
{
    return (m_str + N);
}


//-----------------------------------------------------------------------------------------------------------
template<typename T>
class basic_constexpr_string<T, 0> final
{
public:
    typedef basic_constexpr_string this_type;
    typedef T value_type;
    typedef T* pointer;
    typedef T const* const_pointer;
    typedef T& reference;
    typedef T const& const_reference;
    typedef pointer iterator;
    typedef const_pointer const_iterator;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;

    XR_CONSTEXPR_OR_ENUM(size_type, npos, -1);

    XR_CONSTEXPR_CPP14_OR_INLINE basic_constexpr_string() XR_NOEXCEPT;

    template<size_t M>
    explicit XR_CONSTEXPR_CPP14_OR_INLINE basic_constexpr_string(value_type const(&str)[M]) XR_NOEXCEPT;

    XR_CONSTEXPR_CPP14_OR_INLINE basic_constexpr_string(this_type const& rhs) XR_NOEXCEPT = default;
    XR_CONSTEXPR_CPP14_OR_INLINE basic_constexpr_string(this_type&& rhs) XR_NOEXCEPT = default;

    XR_CONSTEXPR_CPP14_OR_INLINE this_type& operator=(this_type const& rhs) XR_NOEXCEPT = default;
    XR_CONSTEXPR_CPP14_OR_INLINE this_type& operator=(this_type&& rhs) XR_NOEXCEPT = default;

    XR_CONSTEXPR_CPP14_OR_INLINE operator eastl::basic_string_view<T>() const XR_NOEXCEPT;

    XR_CONSTEXPR_CPP14_OR_INLINE size_type size() const XR_NOEXCEPT;

    XR_CONSTEXPR_CPP14_OR_INLINE const_iterator begin() const XR_NOEXCEPT;
    XR_CONSTEXPR_CPP14_OR_INLINE const_iterator end() const XR_NOEXCEPT;

    XR_CONSTEXPR_CPP14_OR_INLINE const_pointer c_str() const XR_NOEXCEPT;
}; // class basic_constexpr_string

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<typename T>
XR_CONSTEXPR_CPP14_OR_INLINE
basic_constexpr_string<T, 0>::basic_constexpr_string() XR_NOEXCEPT
{}

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<typename T>
template<size_t M>
XR_CONSTEXPR_CPP14_OR_INLINE
basic_constexpr_string<T, 0>::basic_constexpr_string(value_type const(&str)[M]) XR_NOEXCEPT
{}

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<typename T>
XR_CONSTEXPR_CPP14_OR_INLINE typename basic_constexpr_string<T, 0>::const_pointer
basic_constexpr_string<T, 0>::c_str() const XR_NOEXCEPT
{
    return nullptr;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<typename T>
XR_CONSTEXPR_CPP14_OR_INLINE
basic_constexpr_string<T, 0>::operator eastl::basic_string_view<T>() const XR_NOEXCEPT
{
    return {};
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<typename T>
XR_CONSTEXPR_CPP14_OR_INLINE typename basic_constexpr_string<T, 0>::size_type
basic_constexpr_string<T, 0>::size() const XR_NOEXCEPT
{
    return 0;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<typename T>
XR_CONSTEXPR_CPP14_OR_INLINE typename basic_constexpr_string<T, 0>::const_iterator
basic_constexpr_string<T, 0>::begin() const XR_NOEXCEPT
{
    return nullptr;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<typename T>
XR_CONSTEXPR_CPP14_OR_INLINE typename basic_constexpr_string<T, 0>::const_iterator
basic_constexpr_string<T, 0>::end() const XR_NOEXCEPT
{
    return nullptr;
}

XR_NAMESPACE_END(xr, utils, details)
//-----------------------------------------------------------------------------------------------------------
