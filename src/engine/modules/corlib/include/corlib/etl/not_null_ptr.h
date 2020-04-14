// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/platform.h"
#include "corlib/etl/algorithms/utility.h"

//-----------------------------------------------------------------------------------------------------------
namespace xr::etl
{

//
// not_null
//
// Restricts a pointer or smart pointer to only hold non-null values.
//
// Has zero size overhead over T.
//
// If T is a pointer (i.e. T == U*) then
// - allow construction from U*
// - disallow construction from nullptr_t
// - disallow default construction
// - ensure construction from null U* fails
// - allow implicit conversion to U*
//
template <typename Type>
class not_null final
{
public:
    using value_type = Type;
    using pointer = eastl::add_pointer_t<value_type>;

    template <typename U, typename = eastl::enable_if_t<eastl::is_convertible<U, Type>::value>>
    constexpr explicit not_null(U&& u) noexcept;

    template <typename = typename eastl::enable_if<!eastl::is_same<std::nullptr_t, Type>::value>::type>
    constexpr explicit not_null(value_type u) noexcept;

    template <typename U, typename = typename eastl::enable_if<eastl::is_convertible<U, Type>::value>::type>
    explicit constexpr not_null(const not_null<U>& other) noexcept;

    not_null(not_null&& other) = default;
    not_null(const not_null& other) = default;
    not_null& operator=(const not_null& other) = default;

    constexpr value_type get() const;
    explicit constexpr operator value_type() const;
    constexpr value_type operator->() const;
    constexpr decltype(auto) operator*() const;

    // prevents compilation when someone attempts to assign a null pointer constant
    not_null(std::nullptr_t) = delete;
    not_null& operator=(std::nullptr_t) = delete;

    // unwanted operators...pointers only point to single objects!
    not_null& operator++() = delete;
    not_null& operator--() = delete;
    not_null operator++(int) = delete;
    not_null operator--(int) = delete;
    not_null& operator+=(ptrdiff_t) = delete;
    not_null& operator-=(ptrdiff_t) = delete;
    void operator[](ptrdiff_t) const = delete;

private:
    pointer m_ptr;
};


//-----------------------------------------------------------------------------------------------------------
/**
*/
template <typename Type>
template <typename U, typename>
constexpr not_null<Type>::not_null(U&& u) noexcept
    : m_ptr(eastl::forward<U>(u))
{
    XR_DEBUG_ASSERTION(m_ptr != nullptr);
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
template <typename Type>
template <typename>
constexpr not_null<Type>::not_null(value_type u) noexcept
    : m_ptr(u)
{
    XR_DEBUG_ASSERTION(m_ptr != nullptr);
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
template <typename Type>
template <typename U, typename>
constexpr not_null<Type>::not_null(const not_null<U>& other) noexcept
    : not_null(other.get())
{}

//-----------------------------------------------------------------------------------------------------------
/**
*/
template <typename Type>
constexpr typename not_null<Type>::value_type not_null<Type>::get() const
{
    XR_DEBUG_ASSERTION(m_ptr != nullptr);
    return m_ptr;
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
template<typename Type>
constexpr 
not_null<Type>::operator value_type() const
{
    return get();
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
template<typename Type>
constexpr typename not_null<Type>::value_type 
not_null<Type>::operator->() const
{
    return get();
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
template<typename Type>
constexpr decltype(auto) 
not_null<Type>::operator*() const
{
    return *get();
}

} // namespace xr::etl
//-----------------------------------------------------------------------------------------------------------
