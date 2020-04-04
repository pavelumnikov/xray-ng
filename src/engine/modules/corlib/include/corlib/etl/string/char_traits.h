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

#include "corlib/types.h"
#include "corlib/etl/type_traits.h"
#include "EASTL/internal/char_traits.h"
#include "EASTL/iterator.h"

//------------------------------------------------------------------------------
namespace xr::etl::string
{

// forward declarations
template<typename T> struct char_traits_types;

//------------------------------------------------------------------------------
template<> struct char_traits_types<char>
{
    using char_type = char;
    using int_type = int;
    using off_type = intptr_t;
    using pos_type = size_t;
    using state_type = char;
}; // struct char_traits_types<char>

//------------------------------------------------------------------------------
template<> struct char_traits_types<wchar_t>
{
    typedef wchar_t char_type;
    typedef wchar_t int_type;
    typedef intptr_t off_type;
    typedef size_t pos_type;
    typedef char state_type;
}; // struct char_traits_types<wchar_t>

//------------------------------------------------------------------------------
template<> struct char_traits_types<char16_t>
{
    typedef char16_t char_type;
    typedef uint_least16_t int_type;
    typedef intptr_t off_type;
    typedef size_t pos_type;
    typedef char state_type;
}; // struct char_traits_types<char16_t>

//------------------------------------------------------------------------------
template<> struct char_traits_types<char32_t>
{
    typedef char32_t char_type;
    typedef uint_least32_t int_type;
    typedef intptr_t off_type;
    typedef size_t pos_type;
    typedef char state_type;
}; // struct char_traits_types<char32_t>

//------------------------------------------------------------------------------
template<typename T>
struct char_traits : public char_traits_types<T>
{
    using char_type = typename char_traits_types<T>::char_type;
    using int_type =  typename char_traits_types<T>::int_type;
    using off_type = typename char_traits_types<T>::off_type;
    using pos_type = typename char_traits_types<T>::pos_type;
    using state_type = typename char_traits_types<T>::state_type;
    using size_type = size_t;

    using pointer = eastl::add_pointer_t<char_type>;
    using const_pointer = eastl::add_pointer_t<eastl::add_const_t<char_type>>;
    using reference = eastl::add_reference_t<char_type>;
    using const_reference = eastl::add_reference_t<eastl::add_const_t<char_type>>;

    static constexpr bool eq(char_type a, char_type b);
    static constexpr bool lt(char_type a, char_type b);
    static constexpr size_t length(const_pointer str);
    template<size_t N> static constexpr size_t length(char_type(&)[N]);
    static void assign(reference r, const_reference c);
    static pointer assign(pointer p, size_t n, char_type c);
    static pointer move(pointer dest, const_pointer src, size_t count);
    static pointer copy(pointer dest, const_pointer src, size_t count);
    static constexpr int compare(const_pointer s1, const_pointer s2, size_t count);
    static constexpr const_pointer find(const_pointer p, size_t count, const_reference ch);
    static constexpr char_type to_char_type(int_type c);
    static constexpr int_type to_int_type(char_type c);
    static constexpr bool eq_int_type(int_type c1, int_type c2);
    static constexpr int_type eof();
    static constexpr int_type not_eof(int_type e);
}; // struct char_traits


//------------------------------------------------------------------------------
/**
*/
template<typename T>
constexpr bool
char_traits<T>::eq(char_type a, char_type b)
{
    return a == b;
}

//------------------------------------------------------------------------------
/**
*/
template<typename T>
constexpr bool 
char_traits<T>::lt(char_type a, char_type b)
{
    return a < b;
}

//------------------------------------------------------------------------------
/**
*/
template<typename T>
constexpr size_t
char_traits<T>::length(const_pointer str)
{
    return eastl::CharStrlen(str);
}

//------------------------------------------------------------------------------
/**
*/
template<typename T>
template<size_t N> 
constexpr size_t 
char_traits<T>::length(char_type(&)[N])
{
    return N;
}

//------------------------------------------------------------------------------
/**
*/
template<typename T>
inline void
char_traits<T>::assign(reference r, const_reference c)
{
    r = c;
}

//------------------------------------------------------------------------------
/**
*/
template<typename T>
inline char_traits<T>::pointer
char_traits<T>::assign(pointer p, size_t n, char_type c)
{
    if(p != 0)
        eastl::fill_n(p, n, c);

    return p;
}

//------------------------------------------------------------------------------
/**
*/
template<typename T>
inline char_traits<T>::pointer
char_traits<T>::move(pointer dest, const_pointer src, size_t count)
{
    if((dest < src) || (dest > (src + count)))
    {
        eastl::copy_n(src, src + count, dest);
    }
    else
    {
        eastl::copy_n(eastl::reverse_iterator<char_type*>(src + count),
            count, eastl::reverse_iterator<char_type*>(dest + count));
    }

    return dest;
}

//------------------------------------------------------------------------------
/**
*/
template<typename T>
inline char_traits<T>::pointer
char_traits<T>::copy(pointer dest, const_pointer src, size_t count)
{
    eastl::copy_n(src, src + count, dest);
    return dest;
}

//------------------------------------------------------------------------------
/**
*/
template<typename T>
constexpr int 
char_traits<T>::compare(const_pointer s1, const_pointer s2, size_t count)
{
    return eastl::Compare(s1, s2, count);
}

//------------------------------------------------------------------------------
/**
*/
template<typename T>
constexpr char_traits<T>::const_pointer
char_traits<T>::find(const_pointer p, size_t count, const_reference ch)
{
    return eastl::Find(p, ch, count);
}

//------------------------------------------------------------------------------
/**
*/
template<typename T>
constexpr char_traits<T>::char_type
char_traits<T>::to_char_type(int_type c)
{
    return static_cast<char_type>(c);
}

//------------------------------------------------------------------------------
/**
*/
template<typename T>
constexpr char_traits<T>::int_type
char_traits<T>::to_int_type(char_type c)
{
    return static_cast<int_type>(c);
}

//------------------------------------------------------------------------------
/**
*/
template<typename T>
constexpr bool 
char_traits<T>::eq_int_type(int_type c1, int_type c2)
{
    return (c1 == c2);
}

//------------------------------------------------------------------------------
/**
*/
template<typename T>
constexpr char_traits<T>::int_type 
char_traits<T>::eof()
{
    return -1;
}

//------------------------------------------------------------------------------
/**
*/
template<typename T>
constexpr char_traits<T>::int_type 
char_traits<T>::not_eof(int_type e)
{
    return (e == eof()) ? eof() - 1 : e;
}

//***************************************************************************
/// Alternative strlen for all character types.
//***************************************************************************
template <typename T>
constexpr size_t strlen(const T* t)
{
    return char_traits<T>::length(t);
}

template <typename T, size_t N>
constexpr size_t strlen(T(&arr)[N])
{
    return char_traits<T>::length(arr);
}

} // namespace xr::etl::string
//------------------------------------------------------------------------------
