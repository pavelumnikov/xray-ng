// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/utils/details/char_traits.h"
#include "EASTL/string_view.h"

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, utils, details)

//-----------------------------------------------------------------------------------------------------------
template<typename T>
class basic_buffer_string
{
    static_assert(is_string_v<T>, "input type must be string");

public:
    typedef basic_buffer_string<T> this_type;
    typedef eastl::basic_string_view<T> view_type;
    typedef typename char_traits<T>::value_type value_type;
    typedef typename char_traits<T>::pointer pointer;
    typedef typename char_traits<T>::const_pointer const_pointer;
    typedef typename char_traits<T>::off_type difference_type;
    typedef typename char_traits<T>::size_type size_type;
    typedef pointer iterator;
    typedef const_pointer const_iterator;
    typedef eastl::reverse_iterator<pointer> reverse_iterator;
    typedef eastl::reverse_iterator<const_pointer> const_reverse_iterator;

    XR_CONSTEXPR_OR_ENUM(size_type, npos, -1);

    XR_CONSTEXPR_CPP14_OR_INLINE basic_buffer_string(pointer buffer, size_type size);
    XR_CONSTEXPR_CPP14_OR_INLINE basic_buffer_string(pointer buffer, size_type size, size_type max_size);
    XR_CONSTEXPR_CPP14_OR_INLINE basic_buffer_string(iterator begin, iterator end);
    XR_CONSTEXPR_CPP14_OR_INLINE basic_buffer_string(iterator begin, iterator max_end, iterator current_end);
    template<size_t N> XR_CONSTEXPR_CPP14_OR_INLINE basic_buffer_string(value_type(&arr)[N]);

    XR_CONSTEXPR_CPP14_OR_INLINE basic_buffer_string(basic_buffer_string&& ref);
    basic_buffer_string& operator=(basic_buffer_string&& ref);

    XR_DECLARE_DELETE_COPY_ASSIGNMENT(basic_buffer_string);

    operator view_type() const;

    // Iterators.

    iterator begin() XR_NOEXCEPT;
    const_iterator begin() const XR_NOEXCEPT;
    const_iterator cbegin() const XR_NOEXCEPT;

    iterator end() XR_NOEXCEPT;
    const_iterator end() const XR_NOEXCEPT;
    const_iterator cend() const XR_NOEXCEPT;

    reverse_iterator rbegin() XR_NOEXCEPT;
    const_reverse_iterator rbegin() const XR_NOEXCEPT;
    const_reverse_iterator crbegin() const XR_NOEXCEPT;

    reverse_iterator rend() XR_NOEXCEPT;
    const_reverse_iterator rend() const XR_NOEXCEPT;
    const_reverse_iterator crend() const XR_NOEXCEPT;

    // Size-related functionality

    bool empty() const XR_NOEXCEPT;
    size_type size() const XR_NOEXCEPT;
    size_type length() const XR_NOEXCEPT;
    size_type max_size() const XR_NOEXCEPT;
    size_type capacity() const XR_NOEXCEPT;

    // Append operations

    this_type& append(const this_type& x);
    this_type& append(const_pointer p, size_type n);
    this_type& append(const_pointer p);
    this_type& append(const_pointer begin, const_pointer end);

private:
    void reset_this();

    pointer m_begin;
    pointer m_current;
    pointer m_end;
}; // class basic_buffer_string

//-----------------------------------------------------------------------------------------------------------
/**
*/
template<typename T>
XR_CONSTEXPR_CPP14_OR_INLINE basic_buffer_string<T>::basic_buffer_string(pointer buffer, size_type max_size)
    : m_begin { buffer }
    , m_current { buffer }
    , m_end { buffer + max_size }
{}

//-----------------------------------------------------------------------------------------------------------
/**
*/
template<typename T>
XR_CONSTEXPR_CPP14_OR_INLINE basic_buffer_string<T>::basic_buffer_string(pointer buffer, size_type size, size_type max_size)
    : m_begin { buffer }
    , m_current { buffer + size }
    , m_end { buffer + max_size }
{}

//-----------------------------------------------------------------------------------------------------------
/**
*/
template<typename T>
XR_CONSTEXPR_CPP14_OR_INLINE basic_buffer_string<T>::basic_buffer_string(iterator begin, iterator end)
    : m_begin { begin }
    , m_current { begin }
    , m_end { end }
{}

//-----------------------------------------------------------------------------------------------------------
/**
*/
template<typename T>
XR_CONSTEXPR_CPP14_OR_INLINE basic_buffer_string<T>::basic_buffer_string(iterator begin, iterator max_end, iterator current_end)
    : m_begin { begin }
    , m_current { current_end }
    , m_end { max_end }
{}

//-----------------------------------------------------------------------------------------------------------
/**
*/
template<typename T>
template<size_t N>
XR_CONSTEXPR_CPP14_OR_INLINE basic_buffer_string<T>::basic_buffer_string(value_type(&arr)[N])
    : m_begin { arr }
    , m_current { arr }
    , m_end { arr + strlen<T>(arr) }
{}

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<typename T>
XR_CONSTEXPR_CPP14_OR_INLINE basic_buffer_string<T>::basic_buffer_string(basic_buffer_string&& ref)
    : m_begin { ref.m_begin }
    , m_current { ref.m_current }
    , m_end { ref.m_end }
{
    ref.reset_this();
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<typename T>
inline basic_buffer_string<T>&
basic_buffer_string<T>::operator=(basic_buffer_string&& ref)
{
    m_begin = ref.m_begin;
    m_current = ref.m_current;
    m_end = ref.m_end;

    ref.reset_this();
    return *this;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<typename T>
inline basic_buffer_string<T>::operator view_type() const
{
    return view_type { m_begin, m_current };
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<typename T>
inline typename basic_buffer_string<T>::iterator 
basic_buffer_string<T>::begin() XR_NOEXCEPT
{
    return m_begin;
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
template<typename T>
inline typename basic_buffer_string<T>::const_iterator 
basic_buffer_string<T>::begin() const XR_NOEXCEPT
{
    return m_begin;
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
template<typename T>
inline typename basic_buffer_string<T>::const_iterator 
basic_buffer_string<T>::cbegin() const XR_NOEXCEPT
{
    return m_begin;
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
template<typename T>
inline typename basic_buffer_string<T>::iterator 
basic_buffer_string<T>::end() XR_NOEXCEPT
{
    return m_end;
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
template<typename T>
inline typename basic_buffer_string<T>::const_iterator
basic_buffer_string<T>::end() const XR_NOEXCEPT
{
    return m_end;
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
template<typename T>
inline typename basic_buffer_string<T>::const_iterator 
basic_buffer_string<T>::cend() const XR_NOEXCEPT
{
    return m_end;
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
template<typename T>
inline typename basic_buffer_string<T>::reverse_iterator 
basic_buffer_string<T>::rbegin() XR_NOEXCEPT
{
    return reverse_iterator(m_end);
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
template<typename T>
inline typename basic_buffer_string<T>::const_reverse_iterator 
basic_buffer_string<T>::rbegin() const XR_NOEXCEPT
{
    return const_reverse_iterator(m_end);
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
template<typename T>
inline typename basic_buffer_string<T>::const_reverse_iterator
basic_buffer_string<T>::crbegin() const XR_NOEXCEPT
{
    return const_reverse_iterator(m_end);
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
template<typename T>
inline typename basic_buffer_string<T>::reverse_iterator 
basic_buffer_string<T>::rend() XR_NOEXCEPT
{
    return reverse_iterator(m_begin);
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
template<typename T>
inline typename basic_buffer_string<T>::const_reverse_iterator
basic_buffer_string<T>::rend() const XR_NOEXCEPT
{
    return const_reverse_iterator(m_begin);
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
template<typename T>
inline typename basic_buffer_string<T>::const_reverse_iterator
basic_buffer_string<T>::crend() const XR_NOEXCEPT
{
    return const_reverse_iterator(m_begin);
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
template<typename T>
inline bool
basic_buffer_string<T>::empty() const XR_NOEXCEPT
{
    return m_begin == m_current;
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
template<typename T>
inline typename basic_buffer_string<T>::size_type 
basic_buffer_string<T>::size() const XR_NOEXCEPT
{
    return static_cast<size_t>(m_current - m_begin);
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
template<typename T>
inline typename basic_buffer_string<T>::size_type 
basic_buffer_string<T>::length() const XR_NOEXCEPT
{
    return size();
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
template<typename T>
inline typename basic_buffer_string<T>::size_type 
basic_buffer_string<T>::max_size() const XR_NOEXCEPT
{
    return static_cast<size_t>(m_end - m_begin);
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
template<typename T>
inline typename basic_buffer_string<T>::size_type 
basic_buffer_string<T>::capacity() const XR_NOEXCEPT
{
    return max_size();
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
template<typename T>
inline typename basic_buffer_string<T>::this_type&
basic_buffer_string<T>::append(const this_type& x)
{
    XR_DEBUG_ASSERTION(x.size() <= (this->capacity() - this->size()));
    pointer new_end = CharStringUninitializedCopy(x.m_begin, x.m_current, m_current);
    *new_end = 0;
    m_current = new_end;
    return *this;
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
template<typename T>
inline typename basic_buffer_string<T>::this_type&
basic_buffer_string<T>::append(const_pointer p, size_type n)
{
    XR_DEBUG_ASSERTION(n <= (this->capacity() - this->size()));
    pointer new_end = CharStringUninitializedCopy(p, reinterpret_cast<pointer>(p + n), m_current);
    *new_end = 0;
    m_current = new_end;
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
template<typename T>
inline typename basic_buffer_string<T>::this_type&
basic_buffer_string<T>::append(const_pointer p)
{
    const size_t n = char_traits<T>::length(p);
    XR_DEBUG_ASSERTION(n <= (this->capacity() - this->size()));
    pointer new_end = eastl::CharStringUninitializedCopy(p, reinterpret_cast<pointer>(p + n), m_current);
    *new_end = 0;
    m_current = new_end;
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
template<typename T>
inline typename basic_buffer_string<T>::this_type&
basic_buffer_string<T>::append(const_pointer begin, const_pointer end)
{
    XR_DEBUG_ASSERTION(size_type(end - begin) <= (this->capacity() - this->size()));
    pointer new_end = eastl::CharStringUninitializedCopy(p, reinterpret_cast<pointer>(p + n), m_current);
    *new_end = 0;
    m_current = new_end;
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
template<typename T>
inline void
basic_buffer_string<T>::reset_this()
{
    m_begin = nullptr;
    m_current = nullptr;
    m_end = nullptr;
}

XR_NAMESPACE_END(xr, utils, details)
//-----------------------------------------------------------------------------------------------------------
