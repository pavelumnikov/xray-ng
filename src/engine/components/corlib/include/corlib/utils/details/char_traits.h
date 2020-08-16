// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/types.h"
#include "corlib/utils/type_traits.h"
#include "corlib/utils/type_inversions.h"
#include "EASTL/internal/char_traits.h"
#include "EASTL/iterator.h"

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, utils, details)

// forward declarations
template<typename T> struct char_traits_types;

//-----------------------------------------------------------------------------------------------------------
template<> struct char_traits_types<char>
{
    typedef char char_type;
    typedef int int_type;
    typedef intptr_t off_type;
    typedef size_t pos_type;
    typedef char state_type;
}; // struct char_traits_types<char>

//-----------------------------------------------------------------------------------------------------------
template<>
struct char_traits_types<wchar_t>
{
    typedef wchar_t char_type;
    typedef wchar_t int_type;
    typedef intptr_t off_type;
    typedef size_t pos_type;
    typedef char state_type;
}; // struct char_traits_types<wchar_t>

//-----------------------------------------------------------------------------------------------------------
template<>
struct char_traits_types<char16_t>
{
    typedef char16_t char_type;
    typedef uint_least16_t int_type;
    typedef intptr_t off_type;
    typedef size_t pos_type;
    typedef char state_type;
}; // struct char_traits_types<char16_t>

//-----------------------------------------------------------------------------------------------------------
template<>
struct char_traits_types<char32_t>
{
    typedef char32_t char_type;
    typedef uint_least32_t int_type;
    typedef intptr_t off_type;
    typedef size_t pos_type;
    typedef char state_type;
}; // struct char_traits_types<char32_t>

//-----------------------------------------------------------------------------------------------------------
template<typename T>
struct char_traits : public char_traits_types<T>
{
    typedef char_traits_types<T> basic_traits;
    typedef typename basic_traits::char_type* pointer;
    typedef typename basic_traits::char_type const* const_pointer;
    typedef typename basic_traits::char_type& reference;
    typedef typename basic_traits::char_type const& const_reference;
    typedef typename basic_traits::char_type value_type;
    typedef typename basic_traits::int_type int_type;
    typedef size_t size_type;

    static XR_CONSTEXPR_CPP14_OR_INLINE bool eq(value_type a, value_type b);
    static XR_CONSTEXPR_CPP14_OR_INLINE bool lt(value_type a, value_type b);
    static XR_CONSTEXPR_CPP14_OR_INLINE size_type length(const_pointer str);
    template<size_t N>
    static XR_CONSTEXPR_CPP14_OR_INLINE size_type constexpr_length(value_type(&)[N]);
    static void assign(reference r, const_reference c);
    static pointer assign(pointer p, size_type n, value_type c);
    static pointer move(pointer dest, const_pointer src, size_type count);
    static pointer copy(pointer dest, const_pointer src, size_type count);
    static XR_CONSTEXPR_CPP14_OR_INLINE int compare(const_pointer s1, const_pointer s2, size_type count);
    static XR_CONSTEXPR_CPP14_OR_INLINE const_pointer find(const_pointer p, size_type count, const_reference ch);
    static XR_CONSTEXPR_CPP14_OR_INLINE value_type to_char_type(int_type c);
    static XR_CONSTEXPR_CPP14_OR_INLINE int_type to_int_type(value_type c);
    static XR_CONSTEXPR_CPP14_OR_INLINE bool eq_int_type(int_type c1, int_type c2);
    static XR_CONSTEXPR_CPP14_OR_INLINE int_type eof();
    static XR_CONSTEXPR_CPP14_OR_INLINE int_type not_eof(int_type e);
}; // struct char_traits


//-----------------------------------------------------------------------------------------------------------
/**
 */
template<typename T>
XR_CONSTEXPR_CPP14_OR_INLINE bool
char_traits<T>::eq(value_type a, value_type b)
{
    return a == b;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<typename T>
XR_CONSTEXPR_CPP14_OR_INLINE bool
char_traits<T>::lt(value_type a, value_type b)
{
    return a < b;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<typename T>
XR_CONSTEXPR_CPP14_OR_INLINE typename char_traits<T>::size_type
char_traits<T>::length(const_pointer str)
{
    return *str ? 1 + length(str + 1) : 0;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<typename T>
template<size_t N> 
XR_CONSTEXPR_CPP14_OR_INLINE typename char_traits<T>::size_type
char_traits<T>::constexpr_length(value_type(&)[N])
{
    return N;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<typename T>
inline void
char_traits<T>::assign(reference r, const_reference c)
{
    r = c;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<typename T>
inline typename char_traits<T>::pointer
char_traits<T>::assign(pointer p, size_type n, value_type c)
{
    if(p != 0)
    {
        for(; n-- > 0; ++p)
            *p = c;
    }

    return p;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<typename T>
inline typename char_traits<T>::pointer
char_traits<T>::move(pointer dest, const_pointer src, size_type count)
{
    if((dest < src) || (dest > (src + count)))
    {
        eastl::copy_n(src, src + count, dest);
    }
    else
    {
        eastl::copy_n(eastl::reverse_iterator<value_type*>(src + count),
            count, eastl::reverse_iterator<value_type*>(dest + count));
    }

    return dest;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<typename T>
inline typename char_traits<T>::pointer
char_traits<T>::copy(pointer dest, const_pointer src, size_type count)
{
    eastl::copy_n(src, src + count, dest);
    return dest;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<typename T>
XR_CONSTEXPR_CPP14_OR_INLINE int
char_traits<T>::compare(const_pointer s1, const_pointer s2, size_type count)
{
    for(; n > 0; ++s1, ++s2, --n)
    {
        if(*s1 != *s2)
            return (static_cast<typename make_unsigned<T>::type>(*s1) <
                static_cast<typename make_unsigned<T>::type>(*s2)) ? -1 : 1;
    }

    return 0;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<typename T>
XR_CONSTEXPR_CPP14_OR_INLINE typename char_traits<T>::const_pointer
char_traits<T>::find(const_pointer p, size_type count, const_reference ch)
{
    for(; count > 0; --count, ++p)
    {
        if(*p == c)
            return p;
    }

    return nullptr;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<typename T>
XR_CONSTEXPR_CPP14_OR_INLINE typename char_traits<T>::value_type
char_traits<T>::to_char_type(int_type c)
{
    return static_cast<value_type>(c);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<typename T>
XR_CONSTEXPR_CPP14_OR_INLINE typename char_traits<T>::int_type
char_traits<T>::to_int_type(value_type c)
{
    return static_cast<int_type>(c);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<typename T>
XR_CONSTEXPR_CPP14_OR_INLINE bool
char_traits<T>::eq_int_type(int_type c1, int_type c2)
{
    return (c1 == c2);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<typename T>
XR_CONSTEXPR_CPP14_OR_INLINE typename char_traits<T>::int_type
char_traits<T>::eof()
{
    return -1;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<typename T>
XR_CONSTEXPR_CPP14_OR_INLINE typename char_traits<T>::int_type
char_traits<T>::not_eof(int_type e)
{
    return (e == eof()) ? eof() - 1 : e;
}

//***************************************************************************
/// Alternative strlen for all character types.
//***************************************************************************
template <typename T>
XR_CONSTEXPR_CPP14_OR_INLINE size_t strlen(T const* t)
{
    return char_traits<T>::length(t);
}

template <typename T, size_t N>
XR_CONSTEXPR_CPP14_OR_INLINE size_t strlen(T(&arr)[N])
{
    return char_traits<T>::length(arr);
}

XR_NAMESPACE_END(xr, utils, details)
//-----------------------------------------------------------------------------------------------------------
