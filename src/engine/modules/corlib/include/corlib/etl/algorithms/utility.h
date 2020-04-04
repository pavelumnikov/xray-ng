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

#include "corlib/etl/type_traits.h"
#include "EASTL/functional.h"

//------------------------------------------------------------------------------
namespace xr::etl::algorithms
{

//------------------------------------------------------------------------------
template <typename Iter, typename Compare>
constexpr auto 
min_element(Iter begin, const Iter end, Compare const &compare)
{
    auto result = begin;

    while(begin != end)
    {
        if(compare(*begin, *result))
        {
            result = begin;
        }

        ++begin;
    }

    return result;
}

//------------------------------------------------------------------------------
template<typename Type, typename ... Args>
void
invoke_as_function(Type t, Args ... args)
{
    static_assert(eastl::is_pointer_v<Type> || eastl::is_reference<Type>::value, 
        "unspported functor type");

    if constexpr(eastl::is_pointer_v<Type>)
        t->operator()(eastl::forward<Args>(args)...);
    else if(eastl::is_reference<Type>::value)
        t.operator()(eastl::forward<Args>(args)...);
}


/// This function creates a eastl::function by binding @a object to the member
/// function pointer @a method.
template<class Instance, class Class, class R, class... Args>
eastl::function<R(Args...)> bind(Instance &object, R(Class::*method)(Args...))
{
    return [&object, method](Args... args) { return (object.*method)(args...); };
}

/// This function creates a eastl::function by binding @a object to the member
/// function pointer @a method.
template<class Class, class R, class... Args>
eastl::function<R(Args...)> bind(Class *object, R(Class::*method)(Args...))
{
    return [object, method](Args... args) { return (object->*method)(args...); };
}

/// This function creates a eastl::function by binding @a object to the member
/// for using with simple function
template<class Class, typename... Args>
eastl::function<void()> bind_to_simple(Class& object, void(Class::* method)(Args...), Args&&... args)
{
    return [object, method, args...]()
    {
        (object->*method)(eastl::forward<Args>(args)...);
    };
}

// workaround: std utility functions aren't constexpr yet
template <class T>
constexpr T&&
constexpr_forward(eastl::remove_reference_t<T>& t) noexcept
{
    return static_cast<T&&>(t);
}

template <class T>
constexpr T&&
constexpr_forward(eastl::remove_reference_t<T>&& t) noexcept
{
    static_assert(!eastl::is_lvalue_reference<T>::value, "!!");
    return static_cast<T&&>(t);
}

template <class T>
constexpr eastl::remove_reference_t<T>&&
constexpr_move(T&& t) noexcept
{
    return static_cast<typename std::remove_reference<T>::type&&>(t);
}

} // namespace xr::etl::algorithms
//------------------------------------------------------------------------------