// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/types.h"
#include "EASTL/type_traits.h"
#include "EASTL/array.h"

//-----------------------------------------------------------------------------------------------------------
namespace xr::etl
{

struct void_value
{};

// Helper type that can be cast-to from any type.
struct any
{
    template<typename T>
    any(T&&) noexcept
    {}
};

template <typename>
struct is_span final : eastl::false_type
{};

template <typename>
struct is_std_array final : eastl::false_type
{};

template <typename T, size_t N>
struct is_std_array<eastl::array<T, N>> final : eastl::true_type
{};

template <typename, typename = void>
struct has_size_and_data final : eastl::false_type
{};

template <typename T>
struct has_size_and_data<T, 
    eastl::void_t<decltype(eastl::size(eastl::declval<T>())), decltype(eastl::data(eastl::declval<T>()))>> final
    : eastl::true_type
{};

template <typename C, typename U = eastl::remove_cvref_t<C>>
struct is_container
{
    static constexpr bool value =
        !is_span<U>::value && !is_std_array<U>::value &&
        !eastl::is_array<U>::value && has_size_and_data<C>::value;
};

template <typename, typename, typename = void>
struct is_container_element_type_compatible final : false_type
{};

template <typename T, typename E>
struct is_container_element_type_compatible<T, E, eastl::void_t<decltype(eastl::data(eastl::declval<T>()))>>final
    : eastl::is_convertible<eastl::remove_pointer_t<decltype(eastl::data(eastl::declval<T>()))>(*)[], E(*)[]>
{};

template <typename, typename = size_t>
struct is_complete final : false_type
{};

template <typename T>
struct is_complete<T, decltype(sizeof(T))> : true_type
{};

template<typename T>
struct is_string final : eastl::false_type
{};

template<>
struct is_string<char>final : eastl::true_type
{};

template<>
struct is_string<wchar_t>final : eastl::true_type
{};

template<>
struct is_string<char16_t>final : eastl::true_type
{};

template<>
struct is_string<char32_t>final : eastl::true_type
{};

template<typename T>
auto constexpr is_string_v = is_string<T>::value;

template<typename T>
struct unwrap_reference
{
    using type = T;
};

template<typename T>
struct unwrap_reference<eastl::reference_wrapper<T>>
{
    using type = T;
};

template<typename T>
using unwrap_reference_t = typename unwrap_reference<T>::type;

template<typename T>
struct remove_rvalue_reference
{
    using type = T;
};

template<typename T>
struct remove_rvalue_reference<T&&>
{
    using type = T;
};

template<typename T>
using remove_rvalue_reference_t = typename remove_rvalue_reference<T>::type;

} // namespace xr::etl
//-----------------------------------------------------------------------------------------------------------
