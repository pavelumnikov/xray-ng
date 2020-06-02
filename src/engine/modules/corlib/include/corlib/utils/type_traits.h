// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/types.h"
#include "EASTL/type_traits.h"
#include "EASTL/array.h"

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, utils)

//-----------------------------------------------------------------------------------------------------------
struct void_value
{};

//-----------------------------------------------------------------------------------------------------------
// Helper type that can be cast-to from any type.
struct any
{
    template<typename T>
    any(T&&) XR_NOEXCEPT
    {}
};

//-----------------------------------------------------------------------------------------------------------
template <typename T>
struct is_span final : eastl::false_type
{};

//-----------------------------------------------------------------------------------------------------------
template <typename T, typename = void>
struct is_std_array : eastl::false_type
{};

//-----------------------------------------------------------------------------------------------------------
template <typename, typename = void>
struct has_size_and_data final : eastl::false_type
{};

//-----------------------------------------------------------------------------------------------------------
template <typename T>
struct has_size_and_data<T, 
    eastl::void_t<decltype(eastl::size(eastl::declval<T>())), decltype(eastl::data(eastl::declval<T>()))>> final
    : eastl::true_type
{};

//-----------------------------------------------------------------------------------------------------------
template <typename C, typename U = eastl::remove_cvref_t<C>>
struct is_container
{
    static constexpr bool value =
        !is_span<U>::value && !is_std_array<U>::value &&
        !eastl::is_array<U>::value && has_size_and_data<C>::value;
};

//-----------------------------------------------------------------------------------------------------------
template <typename, typename, typename = void>
struct is_container_element_type_compatible final : eastl::false_type
{};

//-----------------------------------------------------------------------------------------------------------
template <typename T, typename E>
struct is_container_element_type_compatible<T, E, eastl::void_t<decltype(eastl::data(eastl::declval<T>()))>>final
    : eastl::is_convertible<eastl::remove_pointer_t<decltype(eastl::data(eastl::declval<T>()))>(*)[], E(*)[]>
{};

//-----------------------------------------------------------------------------------------------------------
template <typename, typename = size_t>
struct is_complete final : false_type
{};

//-----------------------------------------------------------------------------------------------------------
template <typename T>
struct is_complete<T, decltype(sizeof(T))> : true_type
{};

//-----------------------------------------------------------------------------------------------------------
template<typename T>
struct is_string final : eastl::false_type
{};

//-----------------------------------------------------------------------------------------------------------
template<>
struct is_string<char>final : eastl::true_type
{};

//-----------------------------------------------------------------------------------------------------------
template<>
struct is_string<wchar_t>final : eastl::true_type
{};

//-----------------------------------------------------------------------------------------------------------
template<>
struct is_string<char16_t>final : eastl::true_type
{};

//-----------------------------------------------------------------------------------------------------------
template<>
struct is_string<char32_t>final : eastl::true_type
{};

//-----------------------------------------------------------------------------------------------------------
template<typename T>
auto constexpr is_string_v = is_string<T>::value;

//-----------------------------------------------------------------------------------------------------------
template<typename T>
struct unwrap_reference
{
    using type = T;
};

//-----------------------------------------------------------------------------------------------------------
template<typename T>
struct unwrap_reference<eastl::reference_wrapper<T>>
{
    using type = T;
};

//-----------------------------------------------------------------------------------------------------------
template<typename T>
using unwrap_reference_t = typename unwrap_reference<T>::type;

//-----------------------------------------------------------------------------------------------------------
template<typename T>
struct remove_rvalue_reference
{
    using type = T;
};

//-----------------------------------------------------------------------------------------------------------
template<typename T>
struct remove_rvalue_reference<T&&>
{
    using type = T;
};

//-----------------------------------------------------------------------------------------------------------
template<typename T>
using remove_rvalue_reference_t = typename remove_rvalue_reference<T>::type;

//-----------------------------------------------------------------------------------------------------------
template <typename T>
struct identity
{
    using type = T;
};

//-----------------------------------------------------------------------------------------------------------
template <typename T>
using identity_t = typename identity<T>::type;

//-----------------------------------------------------------------------------------------------------------
struct end_t
{};

//-----------------------------------------------------------------------------------------------------------
template <typename Predicate, typename... Ts>
struct find_if;

//-----------------------------------------------------------------------------------------------------------
template <typename Predicate, typename T, typename... Ts>
struct find_if<Predicate, T, Ts...>
{
    using result = typename Predicate::template type<T>;
    using type = eastl::conditional_t<result::value, T, typename find_if<Predicate, Ts...>::type>;
};

//-----------------------------------------------------------------------------------------------------------
template <typename Predicate, typename T>
struct find_if<Predicate, T>
{
    using result = typename Predicate::template type<T>;
    using type = eastl::conditional_t<result::value, T, end_t>;
};

//-----------------------------------------------------------------------------------------------------------
template <typename Predicate>
struct find_if<Predicate>
{
    using type = end_t;
};

//-----------------------------------------------------------------------------------------------------------
template <typename Predicate, typename... Ts>
using find_if_t = typename find_if<Predicate, Ts...>::type;

//-----------------------------------------------------------------------------------------------------------
template <typename... Ts>
struct typelist
{
    static constexpr size_t size() noexcept
    {
        return sizeof...(Ts);
    }

    template <typename T>
    static constexpr size_t index_of(identity<T>) noexcept
    {
        return index_of_impl<T>(0, typelist<Ts...>());
    }

    template <typename T>
    static constexpr bool contains(identity<T> type) noexcept
    {
        return index_of(type) != npos;
    }

    static constexpr size_t npos = size_t(-1);

private:
    template <typename T, typename H, typename... Hs>
    static constexpr size_t index_of_impl(const size_t index, typelist<H, Hs...>) noexcept
    {
        return eastl::is_same_v<T, H> ? index : index_of_impl<T>(index + 1, typelist<Hs...>());
    }

    template <typename T>
    static constexpr size_t index_of_impl(const size_t, typelist<>) noexcept
    {
        return npos;
    }
};

//-----------------------------------------------------------------------------------------------------------
template <typename T>
struct is_typelist : eastl::false_type
{};

//-----------------------------------------------------------------------------------------------------------
template <typename... Ts>
struct is_typelist<typelist<Ts...>> : eastl::true_type
{};

//-----------------------------------------------------------------------------------------------------------
template <typename T>
constexpr bool is_typelist_v = is_typelist<T>::value;

//-----------------------------------------------------------------------------------------------------------
template <typename T, typename List>
struct ts_prepend;

//-----------------------------------------------------------------------------------------------------------
template <typename T, typename... Ts>
struct ts_prepend<T, typelist<Ts...>>
{
    using type = typelist<T, Ts...>;
};

//-----------------------------------------------------------------------------------------------------------
template <typename T, typename List>
using ts_prepend_t = typename ts_prepend<T, List>::type;

//-----------------------------------------------------------------------------------------------------------
template <typename List, typename T>
struct ts_append;

//-----------------------------------------------------------------------------------------------------------
template <typename T, typename... Ts>
struct ts_append<typelist<Ts...>, T>
{
    using type = typelist<Ts..., T>;
};

//-----------------------------------------------------------------------------------------------------------
template <typename List, typename T>
using ts_append_t = typename ts_append<List, T>::type;

//-----------------------------------------------------------------------------------------------------------
template <typename List>
struct ts_head;

//-----------------------------------------------------------------------------------------------------------
template <typename T, typename... Ts>
struct ts_head<typelist<T, Ts...>>
{
    using type = T;
};

//-----------------------------------------------------------------------------------------------------------
template <typename List>
using ts_head_t = typename ts_head<List>::type;;

//-----------------------------------------------------------------------------------------------------------
template <typename List>
struct ts_tail;

//-----------------------------------------------------------------------------------------------------------
template <typename T, typename... Ts>
struct ts_tail<typelist<T, Ts...>>
{
    using type = typelist<Ts...>;
};

//-----------------------------------------------------------------------------------------------------------
template <>
struct ts_tail<typelist<>>
{
    using type = typelist<>;
};

//-----------------------------------------------------------------------------------------------------------
template <typename List>
using ts_tail_t = typename ts_tail<List>::type;

//-----------------------------------------------------------------------------------------------------------
template <typename List>
struct ts_reverse;

//-----------------------------------------------------------------------------------------------------------
template <typename T, typename... Ts>
struct ts_reverse<typelist<T, Ts...>>
{
    using type = ts_append_t<typename ts_reverse<typelist<Ts...>>::type, T>;
};

//-----------------------------------------------------------------------------------------------------------
template <>
struct ts_reverse<typelist<>>
{
    using type = typelist<>;
};

//-----------------------------------------------------------------------------------------------------------
template <typename List>
using ts_reverse_t = typename ts_reverse<List>::type;

//-----------------------------------------------------------------------------------------------------------
template <typename List, typename T>
struct ts_erase;

//-----------------------------------------------------------------------------------------------------------
template <typename T, typename H, typename... Hs>
struct ts_erase<typelist<H, Hs...>, T>
{
    using next = typename ts_erase<typelist<Hs...>, T>::type;
    using type = eastl::conditional_t<eastl::is_same_v<T, H>, next, ts_prepend_t<H, next>>;
};

//-----------------------------------------------------------------------------------------------------------
template <typename T>
struct ts_erase<typelist<>, T>
{
    using type = typelist<>;
};

//-----------------------------------------------------------------------------------------------------------
template <typename List, typename T>
using ts_erase_t = typename ts_erase<List, T>::type;

//-----------------------------------------------------------------------------------------------------------
namespace detail
{

//-----------------------------------------------------------------------------------------------------------
template <typename List>
struct ts_applier;

//-----------------------------------------------------------------------------------------------------------
template <typename... Ts>
struct ts_applier<typelist<Ts...>>
{
    template <typename Handler>
    static void apply(Handler&& handler)
    {
        [[maybe_unused]]
        const int fold[] = { (handler(identity<Ts>()), 0)..., 0 };
    }
}; // namespace detail

} // detail namespace

//-----------------------------------------------------------------------------------------------------------
template <typename List, typename Handler>
void ts_apply(Handler&& handler)
{
    detail::ts_applier<List>::apply(eastl::forward<Handler>(handler));
}

XR_NAMESPACE_END(xr, utils)
//-----------------------------------------------------------------------------------------------------------
