/*

  Copyright (c) 2019, Pavel Umnikov
  All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:

  * Redistributions of source code must retain the above copyright notice,
    this list of conditions and the following disclaimer.
  * Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.

  THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND ANY
  EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  DISCLAIMED. IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE FOR ANY
  DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
  OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
  DAMAGE.

*/

#pragma once

#include "corlib/platform.h"
#include "EASTL/type_traits.h"
#include "EASTL/initializer_list.h"

namespace xr
{

using pvoid = void*;
using pcvoid = const void*;
using pbyte = char*;
using pcbyte = const char*;
using pstr = char*;
using pcstr = const char*;
using pwstr = wchar_t*;
using pcwstr = const wchar_t*;

struct int128_t
{
    union
    {
        struct
        {
            int64_t lo;
            int64_t hi;
        };
        int64_t ints[2] = { 0, 0 };
    };

    XR_DECLARE_DEFAULT_CONSTEXPR_CLASS(int128_t);
};

using std::initializer_list;

constexpr size_t operator"" _kb(size_t size)
{
    return size << 10;
}

constexpr size_t operator"" _mb(size_t size)
{
    return size << 20;
}

constexpr size_t operator"" _gb(size_t size)
{
    return size << 30;
}

template<typename EType>
constexpr uint32_t enum_count32() noexcept
{
    // this will fail if count not presented in enumeration
    static_assert(uint32_t(EType::count));
    return uint32_t(EType::count);
}

template<typename EType>
constexpr auto enum_count32_v = enum_count32<EType>();


template<typename EType>
constexpr auto enum_count() noexcept
{
    // this will fail if count not presented in enumeration
    static_assert(size_t(EType::count));
    return size_t(EType::count);
}

template<typename EType>
constexpr auto enum_count_v = enum_count<EType>();


template<typename EType>
constexpr uint32_t enum_index32(EType const e) noexcept
{
    // this will fail if count not presented in enumeration
    static_assert(uint32_t(EType::count));
    // this will fail if count not presented in enumeration
    static_assert(uint32_t(e) < uint32_t(EType::count));
    return uint32_t(e);
}

template<typename EType>
constexpr size_t enum_index(EType const e) noexcept
{
    // this will fail if count not presented in enumeration
    static_assert(size_t(EType::count));
    // this will fail if count not presented in enumeration
    static_assert(size_t(e) < size_t(EType::count));
    return size_t(e);
}

// Primary template with a static assertion
// for a meaningful error message
// if it ever gets instantiated.
// We could leave it undefined if we didn't care.

template<typename, typename T>
struct has_serialize final
{
    static_assert(::eastl::integral_constant<T, false>::value,
        "Second template parameter needs to be of function type.");
};

// specialization that does the checking

template<typename C, typename Ret, typename... Args>
struct has_serialize<C, Ret(Args...)> final
{
private:
    template<typename T>
    static constexpr auto check(T*)
        -> typename
        ::eastl::is_same<
        decltype(::eastl::declval<T>().serialize(::eastl::declval<Args>()...)),
        Ret>::type;  // attempt to call it and see if the return type is correct

    template<typename>
    static constexpr ::eastl::false_type check(...);

    typedef decltype(check<C>(nullptr)) type;

public:
    static constexpr bool value = type::value;
};

} // namespace xr
