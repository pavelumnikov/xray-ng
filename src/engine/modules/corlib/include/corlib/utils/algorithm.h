// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/utils/type_traits.h"
#include "EASTL/algorithm.h"

//-----------------------------------------------------------------------------------------------------------
namespace xr::utils
{

template<typename T>
constexpr T numeric_add(T _a, T _b)
{
    return _a + _b;
}

template<typename T>
constexpr T numeric_sub(T _a, T _b)
{
    return _a - _b;
}

template<typename T>
constexpr T numeric_cmplt(T _a, T _b)
{
    return -(_a < _b);
}

template<typename T>
constexpr T numeric_cmple(T _a, T _b)
{
    return -(_a <= _b);
}

template<typename T>
constexpr T numeric_cmpgt(T _a, T _b)
{
    return -(_a > _b);
}

template<typename T>
constexpr T numeric_cmpge(T _a, T _b)
{
    return -(_a >= _b);
}

template<typename T>
constexpr T numeric_and(T _a, T _b)
{
    return _a & _b;
}

template<typename T>
constexpr T numeric_andc(T _a, T _b)
{
    return _a & ~_b;
}

template<typename T>
constexpr T numeric_xor(T _a, T _b)
{
    return _a ^ _b;
}

template<typename T>
constexpr T numeric_xorl(T _a, T _b)
{
    return !_a != !_b;
}

template<typename T>
constexpr T numeric_or(T _a, T _b)
{
    return _a | _b;
}

template<typename T>
constexpr T numeric_orc(T _a, T _b)
{
    return _a | ~_b;
}

template<typename T>
constexpr T numeric_saturate_add(T _a, T _b)
{
    const T add = numeric_add(_a, _b);
    const T lt = numeric_cmplt(add, _a);
    const T result = numeric_or(add, lt);
    return result;
}

template<typename T>
constexpr T numeric_saturate_sub(T _a, T _b)
{
    const T sub = numeric_sub(_a, _b);
    const T le = numeric_cmple(sub, _a);
    const T result = numeric_and(sub, le);
    return result;
}

} // namespace xr::utils
//-----------------------------------------------------------------------------------------------------------
