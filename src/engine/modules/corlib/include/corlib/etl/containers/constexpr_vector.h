// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/types.h"
#include "corlib/etl/algorithms/utility.h"

//------------------------------------------------------------------------------
namespace xr::etl::containers
{

//------------------------------------------------------------------------------
template<typename T, size_t N>
class constexpr_vector
{
    static_assert(N > 0, "vector must have at lest one element");

public:
    // Container typdefs

    using value_type = T;
    using reference = value_type & ;
    using const_reference = const value_type &;
    using pointer = value_type * ;
    using const_pointer = const value_type *;
    using iterator = pointer;
    using const_iterator = const_pointer;
    using reverse_iterator = ::eastl::reverse_iterator<iterator>;
    using const_reverse_iterator = ::eastl::reverse_iterator<const_iterator>;
    using size_type = size_t;
    using difference_type = ptrdiff_t;

public:
    // Constructors

    constexpr constexpr_vector() = default;
    constexpr constexpr_vector(size_type count, const_reference value);

    // Iterators

    constexpr iterator begin() noexcept;
    constexpr const_iterator begin() const noexcept;
    constexpr const_iterator cbegin() const noexcept;
    constexpr iterator end() noexcept;
    constexpr const_iterator end() const noexcept;
    constexpr const_iterator cend() const noexcept;

    constexpr reverse_iterator rbegin() noexcept;
    constexpr const_reverse_iterator rbegin() const noexcept;
    constexpr const_reverse_iterator crbegin() const noexcept;
    constexpr reverse_iterator rend() noexcept;
    constexpr const_reverse_iterator rend() const noexcept;
    constexpr const_reverse_iterator crend() const noexcept;

    // Capacity

    constexpr size_type size() const;

    // Element access

    constexpr reference operator[](size_type index);
    constexpr const_reference operator[](size_type index) const;

    constexpr reference front();
    constexpr const_reference front() const;

    constexpr reference back();
    constexpr const_reference back() const;

    // Modifiers

    constexpr void push_back(const_reference a);
    constexpr void push_back(T&& a);
    constexpr void pop_back();
    constexpr void clear();

private:
    // zero-initialization for scalar type T, default-initialized otherwise
    value_type m_data[N] = {};
    size_type m_size = 0;
};

//------------------------------------------------------------------------------
/**
*/
template<typename T, size_t N>
constexpr 
constexpr_vector<T, N>::constexpr_vector(size_type count, const_reference value) 
    : m_size(count)
{
    for(size_type i = 0; i < N; ++i)
    {
        this->m_data[i] = value;
    }
}

//------------------------------------------------------------------------------
/**
*/
template<typename T, size_t N>
constexpr typename constexpr_vector<T, N>::iterator
constexpr_vector<T, N>::begin() noexcept
{
    return this->m_data;
}

//------------------------------------------------------------------------------
/**
*/
template<typename T, size_t N>
constexpr typename constexpr_vector<T, N>::const_iterator
constexpr_vector<T, N>::begin() const noexcept
{
    return this->m_data;
}

//------------------------------------------------------------------------------
/**
*/
template<typename T, size_t N>
constexpr typename constexpr_vector<T, N>::const_iterator
constexpr_vector<T, N>::cbegin() const noexcept
{
    return this->m_data;
}

//------------------------------------------------------------------------------
/**
*/
template<typename T, size_t N>
constexpr typename constexpr_vector<T, N>::iterator
constexpr_vector<T, N>::end() noexcept
{
    return this->m_data + this->m_size;
}

//------------------------------------------------------------------------------
/**
*/
template<typename T, size_t N>
constexpr typename constexpr_vector<T, N>::const_iterator
constexpr_vector<T, N>::end() const noexcept
{
    return this->m_data + this->m_size;
}

//------------------------------------------------------------------------------
/**
*/
template<typename T, size_t N>
constexpr typename constexpr_vector<T, N>::const_iterator
constexpr_vector<T, N>::cend() const noexcept
{
    return this->m_data + this->m_size;
}

//------------------------------------------------------------------------------
/**
*/
template<typename T, size_t N>
constexpr typename constexpr_vector<T, N>::reverse_iterator
constexpr_vector<T, N>::rbegin() noexcept
{
    return reverse_iterator(end());
}

//------------------------------------------------------------------------------
/**
*/
template<typename T, size_t N>
constexpr typename constexpr_vector<T, N>::const_reverse_iterator
constexpr_vector<T, N>::rbegin() const noexcept
{
    return const_reverse_iterator(end());
}

//------------------------------------------------------------------------------
/**
*/
template<typename T, size_t N>
constexpr typename constexpr_vector<T, N>::const_reverse_iterator
constexpr_vector<T, N>::crbegin() const noexcept
{
    return const_reverse_iterator(end());
}

//------------------------------------------------------------------------------
/**
*/
template<typename T, size_t N>
constexpr typename constexpr_vector<T, N>::reverse_iterator
constexpr_vector<T, N>::rend() noexcept
{
    return reverse_iterator(begin());
}

//------------------------------------------------------------------------------
/**
*/
template<typename T, size_t N>
constexpr typename constexpr_vector<T, N>::const_reverse_iterator
constexpr_vector<T, N>::rend() const noexcept
{
    return const_reverse_iterator(begin());
}

//------------------------------------------------------------------------------
/**
*/
template<typename T, size_t N>
constexpr typename constexpr_vector<T, N>::const_reverse_iterator
constexpr_vector<T, N>::crend() const noexcept
{
    return const_reverse_iterator(begin());
}

//------------------------------------------------------------------------------
/**
*/
template<typename T, size_t N>
constexpr typename constexpr_vector<T, N>::size_type
constexpr_vector<T, N>::size() const
{
    return m_size;
}

//------------------------------------------------------------------------------
/**
*/
template<typename T, size_t N>
constexpr typename constexpr_vector<T, N>::reference
constexpr_vector<T, N>::operator[](size_type index)
{
    return this->m_data[index];
}

//------------------------------------------------------------------------------
/**
*/
template<typename T, size_t N>
constexpr typename constexpr_vector<T, N>::const_reference
constexpr_vector<T, N>::operator[](size_type index) const
{
    return this->m_data[index];
}

//------------------------------------------------------------------------------
/**
*/
template<typename T, size_t N>
constexpr typename constexpr_vector<T, N>::reference
constexpr_vector<T, N>::front()
{
    return this->m_data[0];
}

//------------------------------------------------------------------------------
/**
*/
template<typename T, size_t N>
constexpr typename constexpr_vector<T, N>::const_reference
constexpr_vector<T, N>::front() const
{
    return this->m_data[0];
}

//------------------------------------------------------------------------------
/**
*/
template<typename T, size_t N>
constexpr typename constexpr_vector<T, N>::reference
constexpr_vector<T, N>::back()
{
    return this->m_data[m_size - 1];
}

//------------------------------------------------------------------------------
/**
*/
template<typename T, size_t N>
constexpr typename constexpr_vector<T, N>::const_reference
constexpr_vector<T, N>::back() const
{
    return this->m_data[this->m_size - 1];
}

//------------------------------------------------------------------------------
/**
*/
template<typename T, size_t N>
constexpr void 
constexpr_vector<T, N>::push_back(const_reference a)
{
    this->m_data[this->m_size++] = a;
}

//------------------------------------------------------------------------------
/**
*/
template<typename T, size_t N>
constexpr void 
constexpr_vector<T, N>::push_back(T&& a)
{
    this->m_data[this->m_size++] = std::move(a);
}

//------------------------------------------------------------------------------
/**
*/
template<typename T, size_t N>
constexpr void 
constexpr_vector<T, N>::pop_back()
{
    --this->m_size;
}

//------------------------------------------------------------------------------
/**
*/
template<typename T, size_t N>
constexpr void 
constexpr_vector<T, N>::clear()
{
    this->m_size = 0;
}

} // namespace xr
//------------------------------------------------------------------------------