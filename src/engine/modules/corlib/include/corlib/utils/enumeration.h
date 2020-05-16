// This file is a part of xray-ng engine
//

// This is modified version of third-party software/middleware for the engine.
// See license of original code below.
//

//  __  __             _        ______                          _____
// |  \/  |           (_)      |  ____|                        / ____|_     _
// | \  / | __ _  __ _ _  ___  | |__   _ __  _   _ _ __ ___   | |   _| |_ _| |_
// | |\/| |/ _` |/ _` | |/ __| |  __| | '_ \| | | | '_ ` _ \  | |  |_   _|_   _|
// | |  | | (_| | (_| | | (__  | |____| | | | |_| | | | | | | | |____|_|   |_|
// |_|  |_|\__,_|\__, |_|\___| |______|_| |_|\__,_|_| |_| |_|  \_____|
//                __/ | https://github.com/Neargye/magic_enum
//               |___/  version 0.6.5
//
// Licensed under the MIT License <http://opensource.org/licenses/MIT>.
// SPDX-License-Identifier: MIT
// Copyright (c) 2019 - 2020 Daniil Goncharov <neargye@gmail.com>.
//
// Permission is hereby  granted, free of charge, to any  person obtaining a copy
// of this software and associated  documentation files (the "Software"), to deal
// in the Software  without restriction, including without  limitation the rights
// to  use, copy,  modify, merge,  publish, distribute,  sublicense, and/or  sell
// copies  of  the Software,  and  to  permit persons  to  whom  the Software  is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE  IS PROVIDED "AS  IS", WITHOUT WARRANTY  OF ANY KIND,  EXPRESS OR
// IMPLIED,  INCLUDING BUT  NOT  LIMITED TO  THE  WARRANTIES OF  MERCHANTABILITY,
// FITNESS FOR  A PARTICULAR PURPOSE AND  NONINFRINGEMENT. IN NO EVENT  SHALL THE
// AUTHORS  OR COPYRIGHT  HOLDERS  BE  LIABLE FOR  ANY  CLAIM,  DAMAGES OR  OTHER
// LIABILITY, WHETHER IN AN ACTION OF  CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE  OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#pragma once

#include "corlib/types.h"
#include "corlib/utils/type_conversions.h"
#include "corlib/utils/constexpr_string.h"
#include "EASTL/optional.h"
#include "EASTL/array.h"

// Checks magic_enum compiler compatibility.
#if defined(__clang__) || defined(__GNUC__) && __GNUC__ >= 9 || defined(_MSC_VER)
#   define MAGIC_ENUM_SUPPORTED 1
#endif

//-----------------------------------------------------------------------------------------------------------
namespace xr::utils
{

// Enum value must be in range [MAGIC_ENUM_RANGE_MIN, MAGIC_ENUM_RANGE_MAX]. By default MAGIC_ENUM_RANGE_MIN = -128, MAGIC_ENUM_RANGE_MAX = 128.
// If need another range for all enum types by default, redefine the macro MAGIC_ENUM_RANGE_MIN and MAGIC_ENUM_RANGE_MAX.
// If need another range for specific enum type, add specialization enum_range for necessary enum type.
template <typename E>
struct enum_range
{
    static_assert(eastl::is_enum_v<E>, "enum_range requires enum type.");
    inline static constexpr int min = -128;
    inline static constexpr int max = 128;
    static_assert(max > min, "enum_range requires max > min.");

    static_assert(min <= 0, "min must be less or equals than 0.");
    static_assert(min > (eastl::numeric_limits<int16_t>::min)(), "min must be greater than INT16_MIN.");

    static_assert(max > 0, "max must be greater than 0.");
    static_assert(max < (eastl::numeric_limits<int16_t>::max)(), "max must be less than INT16_MAX.");
};

namespace detail
{

template <typename T>
struct supported
#if defined(MAGIC_ENUM_SUPPORTED) && MAGIC_ENUM_SUPPORTED || defined(MAGIC_ENUM_NO_CHECK_SUPPORT)
    : eastl::true_type {};
#else
    : eastl::false_type {};
#endif

template <typename T>
inline constexpr bool is_enum_v = eastl::is_enum_v<T> && eastl::is_same_v<T, eastl::decay_t<T>>;

struct char_equal
{
    constexpr bool operator()(char lhs, char rhs) const XR_NOEXCEPT
    {
        return lhs == rhs;
    }
};

constexpr string_view pretty_name(eastl::string_view name) XR_NOEXCEPT
{
    for(size_t i = name.size(); i > 0; --i)
    {
        if(!((name[i - 1] >= '0' && name[i - 1] <= '9') ||
            (name[i - 1] >= 'a' && name[i - 1] <= 'z') ||
            (name[i - 1] >= 'A' && name[i - 1] <= 'Z') ||
            (name[i - 1] == '_')))
        {
            name.remove_prefix(i);
            break;
        }
    }

    if(name.size() > 0 && ((name.front() >= 'a' && name.front() <= 'z') ||
        (name.front() >= 'A' && name.front() <= 'Z') ||
        (name.front() == '_')))
    {
        return string_view { name.data(), name.size() };
    }

    return {}; // Invalid name.
}

template <typename BinaryPredicate>
constexpr bool cmp_equal(string_view lhs, string_view rhs, BinaryPredicate p)
{
    if(lhs.size() != rhs.size())
    {
        return false;
    }

    const auto size = lhs.size();
    for(size_t i = 0; i < size; ++i)
    {
        if(!p(lhs[i], rhs[i]))
        {
            return false;
        }
    }

    return true;
}

template <typename L, typename R>
constexpr bool cmp_less(L lhs, R rhs) XR_NOEXCEPT
{
    static_assert(eastl::is_integral_v<L> && eastl::is_integral_v<R>,
        "detail::cmp_less requires integral type.");

    if constexpr(eastl::is_signed_v<L> == eastl::is_signed_v<R>)
    {
        // If same signedness (both signed or both unsigned).
        return lhs < rhs;
    }
    else if constexpr(eastl::is_signed_v<R>)
    {
        // If 'right' is negative, then result is 'false', otherwise cast & compare.
        return rhs > 0 && lhs < static_cast<eastl::make_unsigned_t<R>>(rhs);
    }
    else
    {
        // If 'left' is negative, then result is 'true', otherwise cast & compare.
        return lhs < 0 || static_cast<eastl::make_unsigned_t<L>>(lhs) < rhs;
    }
}

template <typename E>
constexpr auto n() XR_NOEXCEPT
{
    static_assert(is_enum_v<E>, "current constexpr function requires enum type.");

#  if defined(__clang__)
    constexpr string_view name { __PRETTY_FUNCTION__ + 34, sizeof(__PRETTY_FUNCTION__) - 36 };
#  elif defined(__GNUC__)
    constexpr string_view name { __PRETTY_FUNCTION__ + 49, sizeof(__PRETTY_FUNCTION__) - 51 };
#  elif defined(_MSC_VER)
    constexpr string_view name { __FUNCSIG__ + 40, sizeof(__FUNCSIG__) - 57 };
#  endif

    return utils::constexpr_string<name.size()>{name};
}

template <typename E>
inline constexpr auto type_name_v = n<E>();

template <typename E, E V>
constexpr auto n() XR_NOEXCEPT
{
    static_assert(is_enum_v<E>, "current constexpr function requires enum type.");
#  if defined(__clang__) || defined(__GNUC__)
    constexpr auto name = pretty_name({ __PRETTY_FUNCTION__, sizeof(__PRETTY_FUNCTION__) - 2 });
#  elif defined(_MSC_VER)
    constexpr auto name = pretty_name({ __FUNCSIG__, sizeof(__FUNCSIG__) - 17 });
#  endif
    return utils::constexpr_string<name.size()>{name};
}

template <typename E, E V>
inline constexpr auto name_v = n<E, V>();

template <typename E>
constexpr int reflected_min() XR_NOEXCEPT
{
    static_assert(is_enum_v<E>, "detail::reflected_min requires enum type.");
    constexpr auto lhs = enum_range<E>::min;
    static_assert(lhs > (eastl::numeric_limits<int16_t>::min)(),
        "enum_range requires min must be greater than INT16_MIN.");
    constexpr auto rhs = (eastl::numeric_limits<eastl::underlying_type_t<E>>::min)();

    return cmp_less(lhs, rhs) ? rhs : lhs;
}

template <typename E>
constexpr int reflected_max() XR_NOEXCEPT
{
    static_assert(is_enum_v<E>, "detail::reflected_max requires enum type.");
    constexpr auto lhs = enum_range<E>::max;
    static_assert(lhs < (eastl::numeric_limits<int16_t>::max)(),
        "enum_range requires max must be less than INT16_MAX.");
    constexpr auto rhs = (eastl::numeric_limits<eastl::underlying_type_t<E>>::max)();

    return cmp_less(lhs, rhs) ? lhs : rhs;
}

template <typename E>
inline constexpr int reflected_min_v = reflected_min<E>();

template <typename E>
inline constexpr int reflected_max_v = reflected_max<E>();

template <typename E>
constexpr size_t reflected_size() XR_NOEXCEPT
{
    static_assert(is_enum_v<E>, "detail::reflected_size requires enum type.");
    static_assert(reflected_max_v<E> > reflected_min_v<E>, "enum_range requires max > min.");
    constexpr auto size = reflected_max_v<E> -reflected_min_v<E> +1;
    static_assert(size > 0, "enum_range requires valid size.");
    static_assert(size < (eastl::numeric_limits<uint16_t>::max)(), "enum_range requires valid size.");

    return static_cast<size_t>(size);
}

template <typename E, int... I>
constexpr auto values(eastl::integer_sequence<int, I...>) XR_NOEXCEPT
{
    static_assert(is_enum_v<E>, "detail::values requires enum type.");
    constexpr eastl::array<bool, sizeof...(I)> valid { { (n<E, static_cast<E>(I + reflected_min_v<E>)>().size() != 0)... } };
    constexpr int count = ((valid[I] ? 1 : 0) + ...);

    eastl::array<E, count> values {};
    for(int i = 0, v = 0; v < count; ++i)
    {
        if(valid[i])
        {
            values[v++] = static_cast<E>(i + reflected_min_v<E>);
        }
    }

    return values;
}

template <typename E>
inline constexpr auto values_v = values<E>(eastl::make_integer_sequence<int, reflected_size<E>()>{});

template <typename E>
inline constexpr size_t count_v = values_v<E>.size();

template <typename E>
inline constexpr int min_v = values_v<E>.empty() ? 0 : static_cast<int>(values_v<E>.front());

template <typename E>
inline constexpr int max_v = values_v<E>.empty() ? 0 : static_cast<int>(values_v<E>.back());

template <typename E>
constexpr size_t range_size() XR_NOEXCEPT
{
    static_assert(is_enum_v<E>, "detail::range_size requires enum type.");
    constexpr auto size = max_v<E> -min_v<E> +1;
    static_assert(size > 0, "enum_range requires valid size.");
    static_assert(size < (eastl::numeric_limits<uint16_t>::max)(), "enum_range requires valid size.");

    return static_cast<size_t>(size);
}

template <typename E>
inline constexpr size_t range_size_v = range_size<E>();

template <typename E>
using index_t = eastl::conditional_t < range_size_v<E> < (eastl::numeric_limits<uint8_t>::max)(), uint8_t, uint16_t>;

template <typename E>
inline constexpr auto invalid_index_v = (eastl::numeric_limits<index_t<E>>::max)();

template <typename E, int... I>
constexpr auto indexes(eastl::integer_sequence<int, I...>) XR_NOEXCEPT
{
    static_assert(is_enum_v<E>, "detail::indexes requires enum type.");
    index_t<E> i = 0;

    return eastl::array<index_t<E>, sizeof...(I)>{
        {
            ((n<E, static_cast<E>(I + min_v<E>)>().size() != 0) ? i++ : invalid_index_v<E>)...
        }};
}

template <typename E, size_t... I>
constexpr auto names(eastl::index_sequence<I...>) XR_NOEXCEPT
{
    static_assert(is_enum_v<E>, "detail::names requires enum type.");

    return eastl::array<eastl::string_view, sizeof...(I)>{
        {
            name_v<E, values_v<E>[I]>...
        }};
}

template <typename E, size_t... I>
constexpr auto entries(eastl::index_sequence<I...>) XR_NOEXCEPT
{
    static_assert(is_enum_v<E>, "detail::entries requires enum type.");

    return eastl::array<eastl::pair<E, string_view>, sizeof...(I)>{
        {
            {
                values_v<E>[I], name_v<E, values_v<E>[I]>
            }...
        }};
}

template <typename T, typename R>
using enable_if_enum_t = eastl::enable_if_t<eastl::is_enum_v<eastl::decay_t<T>>, R>;

template <typename T, typename Enable = eastl::enable_if_t<eastl::is_enum_v<eastl::decay_t<T>>>>
using enum_concept = T;

template <typename T, bool = eastl::is_enum_v<T>>
struct is_scoped_enum : eastl::false_type
{};

template <typename T>
struct is_scoped_enum<T, true> : eastl::bool_constant<!eastl::is_convertible_v<T, eastl::underlying_type_t<T>>>
{};

template <typename T, bool = eastl::is_enum_v<T>>
struct is_unscoped_enum : eastl::false_type
{};

template <typename T>
struct is_unscoped_enum<T, true> : eastl::bool_constant<eastl::is_convertible_v<T, eastl::underlying_type_t<T>>>
{};

template <typename T, bool = eastl::is_enum_v<eastl::decay_t<T>>>
struct underlying_type
{};

template <typename T>
struct underlying_type<T, true> : eastl::underlying_type<eastl::decay_t<T>>
{};

template <typename E, bool = is_enum_v<E>>
struct enum_traits
{};

template <typename E>
struct enum_traits<E, true>
{
    using type = E;
    using underlying_type = typename detail::underlying_type<E>::type;

    inline static constexpr string_view type_name = detail::type_name_v<E>;

    inline static constexpr bool is_unscoped = detail::is_unscoped_enum<E>::value;
    inline static constexpr bool is_scoped = detail::is_scoped_enum<E>::value;
    inline static constexpr bool is_dense = detail::range_size_v<E> == detail::count_v<E>;
    inline static constexpr bool is_sparse = detail::range_size_v<E> != detail::count_v<E>;

    inline static constexpr size_t count = detail::count_v<E>;
    inline static constexpr eastl::array<E, count> values = detail::values_v<E>;

    inline static constexpr eastl::array<string_view, count> names = 
        detail::names<E>(eastl::make_index_sequence<count_v<E>>{});

    inline static constexpr eastl::array<eastl::pair<E, string_view>, count> entries =
        detail::entries<E>(eastl::make_index_sequence<count_v<E>>{});

    [[nodiscard]] static constexpr bool reflected(E value) XR_NOEXCEPT
    {
        return reflected(static_cast<U>(value));
    }

    [[nodiscard]] static constexpr int index(E value) XR_NOEXCEPT
    {
        return index(static_cast<U>(value));
    }

    [[nodiscard]] static constexpr E value(size_t index) XR_NOEXCEPT
    {
        if constexpr(is_sparse)
        {
            return assert(index < count), values[index];
        }
        else
        {
            return assert(index < count), static_cast<E>(index + min_v<E>);
        }
    }

    [[nodiscard]] static constexpr string_view name(E value) XR_NOEXCEPT
    {
        if(const auto i = index(value); i != -1)
        {
            return names[i];
        }

        return {}; // Value out of range.
    }

private:
    static_assert(is_enum_v<E>, "enum_traits requires enum type.");
    static_assert(supported<E>::value, "unsupported compiler (https://github.com/Neargye/magic_enum#compiler-compatibility).");
    static_assert(count > 0, "enum_range requires enum implementation and valid max and min.");
    using U = underlying_type;
    inline static constexpr auto indexes = detail::indexes<E>(eastl::make_integer_sequence<int, range_size_v<E>>{});

    static constexpr bool reflected(U value) XR_NOEXCEPT
    {
        return value >= static_cast<U>(reflected_min_v<E>) && value <= static_cast<U>(reflected_max_v<E>);
    }

    static constexpr int index(U value) XR_NOEXCEPT
    {
        if(value >= static_cast<U>(min_v<E>) && value <= static_cast<U>(max_v<E>))
        {
            if constexpr(is_sparse)
            {
                if(const auto i = indexes[value - min_v<E>]; i != invalid_index_v<E>)
                {
                    return i;
                }
            }
            else
            {
                return value - min_v<E>;
            }
        }

        return -1; // Value out of range.
    }
};

} // namespace detail

// Checks is magic_enum supported compiler.
inline constexpr bool is_magic_enum_supported = detail::supported<void>::value;

template <typename T>
using Enum = detail::enum_concept<T>;

// Checks whether T is an Unscoped enumeration type.
// Provides the member constant value which is equal to true, if T is an [Unscoped enumeration](https://en.cppreference.com/w/cpp/language/enum#Unscoped_enumeration) type. Otherwise, value is equal to false.
template <typename T>
struct is_unscoped_enum : detail::is_unscoped_enum<T>
{};

template <typename T>
inline constexpr bool is_unscoped_enum_v = is_unscoped_enum<T>::value;

// Checks whether T is an Scoped enumeration type.
// Provides the member constant value which is equal to true, if T is an [Scoped enumeration](https://en.cppreference.com/w/cpp/language/enum#Scoped_enumerations) type. Otherwise, value is equal to false.
template <typename T>
struct is_scoped_enum : detail::is_scoped_enum<T>
{};

template <typename T>
inline constexpr bool is_scoped_enum_v = is_scoped_enum<T>::value;

// If T is a complete enumeration type, provides a member typedef type that names the underlying type of T.
// Otherwise, if T is not an enumeration type, there is no member type. Otherwise (T is an incomplete enumeration type), the program is ill-formed.
template <typename T>
struct underlying_type : detail::underlying_type<T>
{};

template <typename T>
using underlying_type_t = typename underlying_type<T>::type;

// Enum traits defines a compile-time template-based interface to query the properties of enum.
template <typename E>
using enum_traits = detail::enum_traits<eastl::decay_t<E>>;

// Obtains enum value from enum string name.
// Returns eastl::optional with enum value.
template <typename E, typename BinaryPredicate>
[[nodiscard]] constexpr auto enum_cast(string_view value, BinaryPredicate p) 
-> detail::enable_if_enum_t<E, eastl::optional<eastl::decay_t<E>>>
{
    static_assert(eastl::is_invocable_r_v<bool, BinaryPredicate, char, char>,
        "enum_cast requires bool(char, char) invocable predicate.");
    using D = eastl::decay_t<E>;

    if constexpr(detail::range_size_v<D> > detail::count_v<D> * 2)
    {
        for(size_t i = 0; i < enum_traits<D>::count; ++i)
        {
            if(detail::cmp_equal(value, enum_traits<D>::names[i], p))
            {
                return enum_traits<D>::values[i];
            }
        }
    }
    else
    {
        for(auto i = detail::min_v<D>; i <= detail::max_v<D>; ++i)
        {
            if(detail::cmp_equal(value, enum_traits<D>::name(static_cast<D>(i)), p))
            {
                return static_cast<D>(i);
            }
        }
    }

    return eastl::nullopt; // Invalid value or out of range.
}

template <typename E>
[[nodiscard]] constexpr auto enum_cast(string_view value) XR_NOEXCEPT -> detail::enable_if_enum_t<E, eastl::optional<eastl::decay_t<E>>>
{
    return enum_cast<E>(value, detail::char_equal {});
}

// Obtains enum value from integer value.
// Returns eastl::optional with enum value.
template <typename E>
[[nodiscard]] constexpr auto enum_cast(underlying_type_t<E> value) XR_NOEXCEPT -> detail::enable_if_enum_t<E, eastl::optional<eastl::decay_t<E>>>
{
    using D = eastl::decay_t<E>;

    if(enum_traits<D>::index(static_cast<D>(value)) != -1)
    {
        return static_cast<D>(value);
    }

    return eastl::nullopt; // Invalid value or out of range.
}

// Returns integer value from enum value.
template <typename E>
[[nodiscard]] constexpr auto enum_integer(E value) XR_NOEXCEPT -> detail::enable_if_enum_t<E, underlying_type_t<E>>
{
    return static_cast<underlying_type_t<E>>(value);
}

// Obtains index in enum value sequence from enum value.
// Returns eastl::optional with index.
template <typename E>
[[nodiscard]] constexpr auto enum_index(E value) XR_NOEXCEPT -> detail::enable_if_enum_t<E, eastl::optional<size_t>>
{
    if(const auto i = enum_traits<E>::index(value); i != -1)
    {
        return i;
    }

    return eastl::nullopt; // Value out of range.
}

// Checks whether enum contains enumerator with such value.
template <typename E>
[[nodiscard]] constexpr auto enum_contains(E value) XR_NOEXCEPT -> detail::enable_if_enum_t<E, bool>
{
    return enum_traits<E>::index(value) != -1;
}

// Checks whether enum contains enumerator with such integer value.
template <typename E>
[[nodiscard]] constexpr auto enum_contains(underlying_type_t<E> value) XR_NOEXCEPT -> detail::enable_if_enum_t<E, bool>
{
    return enum_cast<E>(value).has_value();
}

// Checks whether enum contains enumerator with such string enum name.
template <typename E>
[[nodiscard]] constexpr auto enum_contains(string_view value) XR_NOEXCEPT -> detail::enable_if_enum_t<E, bool>
{
    return enum_cast<E>(value).has_value();
}

// Returns enum value at specified index.
// No bounds checking is performed: the behavior is undefined if index >= number of enum values.
template <typename E>
[[nodiscard]] constexpr auto enum_value(size_t index) XR_NOEXCEPT -> detail::enable_if_enum_t<E, eastl::decay_t<E>>
{
    return enum_traits<E>::value(index);
}

// Obtains value enum sequence.
// Returns eastl::array with enum values, sorted by enum value.
template <typename E>
[[nodiscard]] constexpr auto enum_values() XR_NOEXCEPT -> detail::enable_if_enum_t<E, decltype(enum_traits<E>::values)&>
{
    return enum_traits<E>::values;
}

// Returns number of enum values.
template <typename E>
[[nodiscard]] constexpr auto enum_count() XR_NOEXCEPT -> detail::enable_if_enum_t<E, size_t>
{
    return enum_traits<E>::count;
}

// Returns string enum name from static storage enum variable.
// This version is much lighter on the compile times and is not restricted to the enum_range limitation.
template <auto V>
[[nodiscard]] constexpr auto enum_name() XR_NOEXCEPT -> detail::enable_if_enum_t<decltype(V), string_view>
{
    constexpr string_view name = detail::name_v<eastl::decay_t<decltype(V)>, V>;
    static_assert(name.size() > 0, "Enum value does not have a name.");

    return name;
}

// Returns string enum name from enum value.
// If enum value does not have name or value out of range, returns empty string.
template <typename E>
[[nodiscard]] constexpr auto enum_name(E value) XR_NOEXCEPT -> detail::enable_if_enum_t<E, string_view>
{
    return enum_traits<E>::name(value);
}

// Obtains string enum name sequence.
// Returns eastl::array with string enum names, sorted by enum value.
template <typename E>
[[nodiscard]] constexpr auto enum_names() XR_NOEXCEPT -> detail::enable_if_enum_t<E, decltype(enum_traits<E>::names)&>
{
    return enum_traits<E>::names;
}

// Obtains pair (value enum, string enum name) sequence.
// Returns eastl::array with eastl::pair (value enum, string enum name), sorted by enum value.
template <typename E>
[[nodiscard]] constexpr auto enum_entries() XR_NOEXCEPT -> detail::enable_if_enum_t<E, decltype(enum_traits<E>::entries)&>
{
    return enum_traits<E>::entries;
}

namespace bitwise_operators
{

template <typename E>
constexpr auto operator~(E rhs) XR_NOEXCEPT -> detail::enable_if_enum_t<E, E>
{
    return static_cast<E>(~static_cast<underlying_type_t<E>>(rhs));
}

template <typename E>
constexpr auto operator|(E lhs, E rhs) XR_NOEXCEPT -> detail::enable_if_enum_t<E, E>
{
    return static_cast<E>(static_cast<underlying_type_t<E>>(lhs) | static_cast<underlying_type_t<E>>(rhs));
}

template <typename E>
constexpr auto operator&(E lhs, E rhs) XR_NOEXCEPT -> detail::enable_if_enum_t<E, E>
{
    return static_cast<E>(static_cast<underlying_type_t<E>>(lhs)& static_cast<underlying_type_t<E>>(rhs));
}

template <typename E>
constexpr auto operator^(E lhs, E rhs) XR_NOEXCEPT -> detail::enable_if_enum_t<E, E>
{
    return static_cast<E>(static_cast<underlying_type_t<E>>(lhs) ^ static_cast<underlying_type_t<E>>(rhs));
}

template <typename E>
constexpr auto operator|=(E& lhs, E rhs) XR_NOEXCEPT -> detail::enable_if_enum_t<E, E&>
{
    return lhs = lhs | rhs;
}

template <typename E>
constexpr auto operator&=(E& lhs, E rhs) XR_NOEXCEPT -> detail::enable_if_enum_t<E, E&>
{
    return lhs = lhs & rhs;
}

template <typename E>
constexpr auto operator^=(E& lhs, E rhs) XR_NOEXCEPT -> detail::enable_if_enum_t<E, E&>
{
    return lhs = lhs ^ rhs;
}

} // namespace bitwise_operators

} // namespace xr::utils
//-----------------------------------------------------------------------------------------------------------
