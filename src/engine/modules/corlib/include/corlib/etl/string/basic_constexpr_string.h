// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/types.h"
#include "EASTL/type_traits.h"

//------------------------------------------------------------------------------
namespace xr::etl::string
{

//------------------------------------------------------------------------------
template<typename T, size_t N>
class basic_constexpr_string final
{
public:
    using this_type = basic_constexpr_string;
    using value_type = T;
    using pointer = eastl::add_pointer_t<T>;
    using const_pointer = eastl::add_pointer_t<eastl::add_const_t<T>>;
    using reference = eastl::add_lvalue_reference_t<T>;
    using const_reference = eastl::add_lvalue_reference_t<eastl::add_const_t<T>>;
    using iterator = pointer;
    using const_iterator = const_pointer;
    using size_type = size_t;
    using difference_type = eastl::make_signed<size_type>::type;

    static constexpr size_type npos = size_type(-1);

    constexpr basic_constexpr_string() = default;

    template<size_t M>
    explicit constexpr basic_constexpr_string(value_type const(&str)[M]) noexcept;

    constexpr basic_constexpr_string(this_type const& rhs) noexcept = default;
    constexpr basic_constexpr_string(this_type&& rhs) noexcept = default;

    constexpr this_type& operator=(this_type const& rhs) noexcept = default;
    constexpr this_type& operator=(this_type&& rhs) noexcept = default;

    constexpr size_type size() const noexcept;

    constexpr const_iterator begin() const noexcept;
    constexpr const_iterator end() const noexcept;

    constexpr const_pointer c_str() const noexcept;

private:
    value_type m_str[N] {};
}; // class basic_constexpr_string

//------------------------------------------------------------------------------
/**
*/
template<typename T, size_t N>
template<size_t M>
constexpr 
basic_constexpr_string<T, N>::basic_constexpr_string(value_type const(&str)[M]) noexcept
{
    static_assert(M <= N, "input string");
    for(size_t i = 0; i < M; ++i)
        m_str[i] = str[i];
}

//------------------------------------------------------------------------------
/**
*/
template<typename T, size_t N>
constexpr typename basic_constexpr_string<T, N>::const_pointer
basic_constexpr_string<T, N>::c_str() const noexcept
{
    return m_str;
}

//------------------------------------------------------------------------------
/**
*/
template<typename T, size_t N>
constexpr typename basic_constexpr_string<T, N>::size_type
basic_constexpr_string<T, N>::size() const noexcept
{
    return N;
}

//------------------------------------------------------------------------------
/**
*/
template<typename T, size_t N>
constexpr typename basic_constexpr_string<T, N>::const_iterator 
basic_constexpr_string<T, N>::begin() const noexcept
{
    return m_str;
}

//------------------------------------------------------------------------------
/**
*/
template<typename T, size_t N>
constexpr typename basic_constexpr_string<T, N>::const_iterator 
basic_constexpr_string<T, N>::end() const noexcept
{
    return (m_str + N);
}

} // namespace xr::etl::string
//------------------------------------------------------------------------------