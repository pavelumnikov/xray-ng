// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/etl/algorithms/utility.h"
#include "corlib/etl/string/char_traits.h"
#include "EASTL/string_view.h"

//-----------------------------------------------------------------------------------------------------------
namespace xr::etl::string
{

//-----------------------------------------------------------------------------------------------------------
template<typename T>
class basic_buffer_string
{
    static_assert(etl::is_string_v<T>, "input type must be string");

public:
    using this_type = basic_buffer_string<T>;
    using view_type = eastl::basic_string_view<T>;
    using value_type = typename char_traits<T>::char_type;
    using pointer = typename char_traits<T>::pointer;
    using const_pointer = typename char_traits<T>::const_pointer;
    using difference_type = typename char_traits<T>::off_type;
    using size_type = typename char_traits<T>::size_type;
    using iterator = pointer;
    using const_iterator = const_pointer;
    using reverse_iterator = std::reverse_iterator<pointer>;
    using const_reverse_iterator = std::reverse_iterator<const_pointer>;

    static constexpr size_type npos = size_type(-1);

    constexpr basic_buffer_string(pointer buffer, size_type size);
    constexpr basic_buffer_string(pointer buffer, size_type size, size_type max_size);
    constexpr basic_buffer_string(iterator begin, iterator end);
    constexpr basic_buffer_string(iterator begin, iterator max_end, iterator current_end);
    template<size_t N> constexpr basic_buffer_string(value_type(&arr)[N]);

    constexpr basic_buffer_string(basic_buffer_string&& ref);
    basic_buffer_string& operator=(basic_buffer_string&& ref);

    XR_DECLARE_DELETE_COPY_ASSIGNMENT(basic_buffer_string);

    operator view_type() const;

    // Iterators.

    iterator begin() noexcept;
    const_iterator begin() const noexcept;
    const_iterator cbegin() const noexcept;

    iterator end() noexcept;
    const_iterator end() const noexcept;
    const_iterator cend() const noexcept;

    reverse_iterator rbegin() noexcept;
    const_reverse_iterator rbegin() const noexcept;
    const_reverse_iterator crbegin() const noexcept;

    reverse_iterator rend() noexcept;
    const_reverse_iterator rend() const noexcept;
    const_reverse_iterator crend() const noexcept;

    // Size-related functionality

    bool empty() const noexcept;
    size_type size() const noexcept;
    size_type length() const noexcept;
    size_type max_size() const noexcept;
    size_type capacity() const noexcept;

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
constexpr basic_buffer_string<T>::basic_buffer_string(pointer buffer, size_type max_size)
    : m_begin { buffer }
    , m_current { buffer }
    , m_end { buffer + max_size }
{}

//-----------------------------------------------------------------------------------------------------------
/**
*/
template<typename T>
constexpr basic_buffer_string<T>::basic_buffer_string(pointer buffer, size_type size, size_type max_size)
    : m_begin { buffer }
    , m_current { buffer + size }
    , m_end { buffer + max_size }
{}

//-----------------------------------------------------------------------------------------------------------
/**
*/
template<typename T>
constexpr basic_buffer_string<T>::basic_buffer_string(iterator begin, iterator end)
    : m_begin { begin }
    , m_current { begin }
    , m_end { end }
{}

//-----------------------------------------------------------------------------------------------------------
/**
*/
template<typename T>
constexpr basic_buffer_string<T>::basic_buffer_string(iterator begin, iterator max_end, iterator current_end)
    : m_begin { begin }
    , m_current { current_end }
    , m_end { max_end }
{}

//-----------------------------------------------------------------------------------------------------------
/**
*/
template<typename T>
template<size_t N>
constexpr basic_buffer_string<T>::basic_buffer_string(value_type(&arr)[N])
    : m_begin { arr }
    , m_current { arr }
    , m_end { arr + strlen<T>(arr) }
{}

//-----------------------------------------------------------------------------------------------------------
/**
*/
template<typename T>
constexpr basic_buffer_string<T>::basic_buffer_string(basic_buffer_string&& ref)
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
constexpr basic_buffer_string<T>::basic_buffer_string(basic_buffer_string const& ref)
    : m_begin { ref.m_begin }
    , m_current { ref.m_current }
    , m_end { ref.m_end }
{}

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
inline basic_buffer_string<T>::iterator 
basic_buffer_string<T>::begin() noexcept
{
    return m_begin;
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
template<typename T>
inline basic_buffer_string<T>::const_iterator 
basic_buffer_string<T>::begin() const noexcept
{
    return m_begin;
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
template<typename T>
inline basic_buffer_string<T>::const_iterator 
basic_buffer_string<T>::cbegin() const noexcept
{
    return m_begin;
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
template<typename T>
inline basic_buffer_string<T>::iterator 
basic_buffer_string<T>::end() noexcept
{
    return m_end;
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
template<typename T>
inline basic_buffer_string<T>::const_iterator
basic_buffer_string<T>::end() const noexcept
{
    return m_end;
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
template<typename T>
inline basic_buffer_string<T>::const_iterator 
basic_buffer_string<T>::cend() const noexcept
{
    return m_end;
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
template<typename T>
inline basic_buffer_string<T>::reverse_iterator 
basic_buffer_string<T>::rbegin() noexcept
{
    return reverse_iterator(m_end);
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
template<typename T>
inline basic_buffer_string<T>::const_reverse_iterator 
basic_buffer_string<T>::rbegin() const noexcept
{
    return const_reverse_iterator(m_end);
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
template<typename T>
inline basic_buffer_string<T>::const_reverse_iterator
basic_buffer_string<T>::crbegin() const noexcept
{
    return const_reverse_iterator(m_end);
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
template<typename T>
inline basic_buffer_string<T>::reverse_iterator 
basic_buffer_string<T>::rend() noexcept
{
    return reverse_iterator(m_begin);
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
template<typename T>
inline basic_buffer_string<T>::const_reverse_iterator
basic_buffer_string<T>::rend() const noexcept
{
    return const_reverse_iterator(m_begin);
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
template<typename T>
inline basic_buffer_string<T>::const_reverse_iterator
basic_buffer_string<T>::crend() const noexcept
{
    return const_reverse_iterator(m_begin);
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
template<typename T>
inline bool
basic_buffer_string<T>::empty() const noexcept
{
    return m_begin == m_current;
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
template<typename T>
inline basic_buffer_string<T>::size_type 
basic_buffer_string<T>::size() const noexcept
{
    return static_cast<size_t>(m_current - m_begin);
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
template<typename T>
inline basic_buffer_string<T>::size_type 
basic_buffer_string<T>::length() const noexcept
{
    return size();
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
template<typename T>
inline basic_buffer_string<T>::size_type 
basic_buffer_string<T>::max_size() const noexcept
{
    return static_cast<size_t>(m_end - m_begin);
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
template<typename T>
inline basic_buffer_string<T>::size_type 
basic_buffer_string<T>::capacity() const noexcept
{
    return max_size();
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
template<typename T>
inline basic_buffer_string<T>::this_type&
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
inline basic_buffer_string<T>::this_type&
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
inline basic_buffer_string<T>::this_type&
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
inline basic_buffer_string<T>::this_type&
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

} // namespace xr::etl::string
//-----------------------------------------------------------------------------------------------------------
