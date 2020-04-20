// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/utils/details/char_traits.h"

//-----------------------------------------------------------------------------------------------------------
namespace xr::utils::details
{

template<typename Type>
class basic_string_view
{
public:
    using string_traits = char_traits<Type>;
    using value_type = typename string_traits::char_type;
    using pointer = typename string_traits::pointer;
    using const_pointer = typename string_traits::const_pointer;
    using reference = typename string_traits::reference;
    using const_reference = typename string_traits::const_reference;
    using iterator = typename string_traits::pointer;
    using const_iterator = typename string_traits::const_pointer;
    using size_type = typename string_traits::size_type;
    using difference_type = typename string_traits::off_type;

    static constexpr size_type npos = size_type(-1);

    constexpr basic_string_view() noexcept;
    constexpr basic_string_view(const_pointer s, size_type count) noexcept;
    constexpr basic_string_view(const_pointer s) noexcept;

    XR_DECLARE_DEFAULT_CONSTEXPR_COPY_ASSIGNMENT(basic_string_view);

    constexpr const_pointer data() const noexcept;
    constexpr value_type operator[](size_type pos) const noexcept;
    constexpr size_type size() const noexcept;
    constexpr bool empty() const noexcept;
    constexpr iterator begin() const noexcept;
    constexpr const_iterator cbegin() const noexcept;
    constexpr iterator end() const noexcept;
    constexpr const_iterator cend() const noexcept;
    constexpr basic_string_view substr(size_type pos = 0, size_type count = npos) const noexcept;
    constexpr void swap(basic_string_view& v) noexcept;

private:
    basic_string_view(decltype(nullptr)) = delete;

    const_pointer m_str;
    size_type m_size;
};

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<typename Type>
constexpr basic_string_view<Type>::basic_string_view() noexcept
    : m_str { nullptr }
    , m_size { 0 }
{}

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<typename Type>
constexpr basic_string_view<Type>::basic_string_view(const_pointer s, size_type count) noexcept
    : m_str { s }
    , m_size { count }
{}

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<typename Type>
constexpr basic_string_view<Type>::basic_string_view(const_pointer s) noexcept
    : m_str { s }
    , m_size { string_traits::length(s) }
{}

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<typename Type>
constexpr typename basic_string_view<Type>::const_pointer basic_string_view<Type>::data() const noexcept
{
    return m_str;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<typename Type>
constexpr typename basic_string_view<Type>::value_type
basic_string_view<Type>::operator[](size_type pos) const noexcept
{
    return m_str[pos];
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<typename Type>
constexpr typename basic_string_view<Type>::size_type basic_string_view<Type>::size() const noexcept
{
    return m_size;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<typename Type>
constexpr bool basic_string_view<Type>::empty() const noexcept
{
    return 0 == m_size;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<typename Type>
constexpr typename basic_string_view<Type>::iterator basic_string_view<Type>::begin() const noexcept
{
    return m_str;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<typename Type>
constexpr typename basic_string_view<Type>::const_iterator basic_string_view<Type>::cbegin() const noexcept
{
    return m_str;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<typename Type>
constexpr typename basic_string_view<Type>::iterator basic_string_view<Type>::end() const noexcept
{
    return m_str + m_size;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<typename Type>
constexpr typename basic_string_view<Type>::const_iterator basic_string_view<Type>::cend() const noexcept
{
    return m_str + m_size;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<typename Type>
constexpr basic_string_view<Type>
basic_string_view<Type>::substr(size_type pos, size_type count) const noexcept
{
    return basic_string_view(m_str + pos, npos == count ? m_size - pos : count);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<typename Type>
constexpr void basic_string_view<Type>::swap(basic_string_view& v) noexcept
{
    const char* strtmp = m_str;
    size_type sizetmp = m_size;
    m_str = v.m_str;
    m_size = v.m_size;
    v.m_str = strtmp;
    v.m_size = sizetmp;
}

} // namespace xr::utils
//-----------------------------------------------------------------------------------------------------------