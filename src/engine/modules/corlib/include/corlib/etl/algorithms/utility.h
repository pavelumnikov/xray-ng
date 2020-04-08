// This file is a part of xray-ng engine
//

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