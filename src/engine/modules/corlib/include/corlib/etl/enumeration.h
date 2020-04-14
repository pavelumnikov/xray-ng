// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/platform.h"
#include "corlib/etl/type_conversions.h"

//-----------------------------------------------------------------------------------------------------------
namespace xr::etl
{

//-----------------------------------------------------------------------------------------------------------
namespace details
{

// Enumeration is a specific enumeration_impl type definition. One main feature 
// is type safety so you can easily convert and compare different enumerations 
// using this definitions.
template<typename T, typename U, U defaultValue>
class enumeration_impl final
{
public:
    static_assert(eastl::is_same_v<U, eastl::underlying_type_t<T>>, "different underlying types");

    using this_type = enumeration_impl<T, U, defaultValue>;
    using value_type = U;
    using enumeration_type = T;

    constexpr enumeration_impl() noexcept;
    explicit constexpr enumeration_impl(enumeration_type e) noexcept;
    ~enumeration_impl() = default;

    XR_DECLARE_DEFAULT_COPY_ASSIGNMENT(enumeration_impl);
    XR_DECLARE_DEFAULT_MOVE_ASSIGNMENT(enumeration_impl);

    constexpr operator enumeration_type() const noexcept;

    enumeration_impl& operator = (enumeration_type const e) noexcept;

    enumeration_impl& operator += (enumeration_type const e) noexcept;
    enumeration_impl& operator -= (enumeration_type const e) noexcept;

    constexpr bool operator == (enumeration_type const e) const noexcept;
    constexpr bool operator != (enumeration_type const e) const noexcept;

    constexpr bool operator <= (enumeration_type const e) const noexcept;
    constexpr bool operator >= (enumeration_type const e) const noexcept;
    constexpr bool operator < (enumeration_type const e) const noexcept;
    constexpr bool operator > (enumeration_type const e) const noexcept;

    constexpr bool test(enumeration_type e) const noexcept;
    constexpr void reset() const noexcept;

private:
    value_type m_storage; //!< TStore of enumerations
};

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<typename T, typename U, U defaultValue>
constexpr enumeration_impl<T, U, defaultValue>::enumeration_impl() noexcept
    : m_storage { defaultValue }
{}

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<typename T, typename U, U defaultValue>
constexpr enumeration_impl<T, U, defaultValue>::enumeration_impl(enumeration_type e) noexcept
    : m_storage(to_underlying(e))
{}

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<typename T, typename U, U defaultValue>
enumeration_impl<T, U, defaultValue>&
enumeration_impl<T, U, defaultValue>::operator = (enumeration_type e) noexcept
{
    this->m_storage = to_underlying(e);
    return *this;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<typename T, typename U, U defaultValue>
constexpr enumeration_impl<T, U, defaultValue>::operator enumeration_type() const noexcept
{
    return static_cast<enumeration_type>(this->m_storage);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<typename T, typename U, U defaultValue>
typename enumeration_impl<T, U, defaultValue>::this_type&
enumeration_impl<T, U, defaultValue>::operator += (const enumeration_type e) noexcept
{
    this->m_storage |= to_underlying(e);
    return *this;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<typename T, typename U, U defaultValue>
typename enumeration_impl<T, U, defaultValue>::this_type&
enumeration_impl<T, U, defaultValue>::operator -= (const enumeration_type e) noexcept
{
    this->m_storage &= ~to_underlying(e);
    return *this;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<typename T, typename U, U defaultValue>
constexpr bool
enumeration_impl<T, U, defaultValue>::operator == (enumeration_type e) const noexcept
{
    return this->m_storage == to_underlying(e);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<typename T, typename U, U defaultValue>
constexpr bool
enumeration_impl<T, U, defaultValue>::operator != (enumeration_type e) const noexcept
{
    return this->m_storage != to_underlying(e);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<typename T, typename U, U defaultValue>
constexpr bool
enumeration_impl<T, U, defaultValue>::operator <= (enumeration_type e) const noexcept
{
    return this->m_storage <= to_underlying(e);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<typename T, typename U, U defaultValue>
constexpr bool
enumeration_impl<T, U, defaultValue>::operator >= (enumeration_type e) const noexcept
{
    return this->m_storage >= to_underlying(e);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<typename T, typename U, U defaultValue>
constexpr bool
enumeration_impl<T, U, defaultValue>::operator < (enumeration_type e) const noexcept
{
    return this->m_storage < to_underlying(e);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<typename T, typename U, U defaultValue>
constexpr bool
enumeration_impl<T, U, defaultValue>::operator > (enumeration_type e) const noexcept
{
    return this->m_storage > to_underlying(e);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<typename T, typename U, U defaultValue>
constexpr bool
enumeration_impl<T, U, defaultValue>::test(enumeration_type e) const noexcept
{
    return (this->m_storage & to_underlying(e)) != 0;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<typename T, typename U, U defaultValue>
constexpr void
enumeration_impl<T, U, defaultValue>::reset() const noexcept
{
    m_storage = static_cast<value_type>(defaultValue);
}

} // namespace details

//-----------------------------------------------------------------------------------------------------------
template<typename T>
using enumeration = details::enumeration_impl<T, eastl::underlying_type_t<T>, 0>;

//-----------------------------------------------------------------------------------------------------------
template<typename T, T defaultValue>
using defaulting_enumeration = details::enumeration_impl<
    T, eastl::underlying_type_t<T>, to_underlying(defaultValue)>;

} // namespace xr::etl
//-----------------------------------------------------------------------------------------------------------
