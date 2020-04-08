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
class constexpr_array
{
    static_assert(N > 0, "array must have at lest one element");

public:
    // Container typdefs

    using value_type = T;
    using reference = value_type & ;
    using const_reference = const value_type &;
    using pointer = value_type * ;
    using const_pointer = const value_type *;
    using iterator = pointer;
    using const_iterator = const_pointer;
    using reverse_iterator = ::std::reverse_iterator<iterator>;
    using const_reverse_iterator = ::std::reverse_iterator<const_iterator>;
    using size_type = size_t;
    using difference_type = ptrdiff_t;

public:
    // Constructors

    constexpr constexpr_array() = default;

    constexpr constexpr_array(std::initializer_list<T> init);

    template <size_t M>
    constexpr constexpr_array(T const (&init)[M]);

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

    constexpr reference at(size_type index);
    constexpr const_reference at(size_type index) const;

    // Utility

    constexpr void fill(const value_type& val);

private:
    template <size_t M, size_t... I>
    constexpr constexpr_array(T const (&init)[M], ::std::index_sequence<I...>)
        : m_data { init[I]... }
    {}

    template <class Iter, size_t... I>
    constexpr constexpr_array(Iter iter, ::std::index_sequence<I...>)
        : m_data { ((void)I, *iter++)... }
    {}

    // zero-initialization for scalar type T, default-initialized otherwise
    value_type m_data[N];
};

//------------------------------------------------------------------------------
/**
*/
template<typename T, size_t N>
constexpr
constexpr_array<T, N>::constexpr_array(std::initializer_list<T> init)
    : constexpr_array(init.begin(), ::std::make_index_sequence<N>())
{
#if defined(_MSC_VER)
    // clang & gcc doesn't recognize init.size() as a constexpr
    static_assert(init.size() >= N, "Cannot initialize a carray with an smaller array");
#endif // defined(_MSC_VER)
}


//------------------------------------------------------------------------------
/**
*/
template<typename T, size_t N>
template <size_t M>
constexpr 
constexpr_array<T, N>::constexpr_array(T const (&init)[M])
    : constexpr_array(init, ::std::make_index_sequence<N>())
{
    static_assert(M >= N, "Cannot initialize a carray with an smaller array");
}

//------------------------------------------------------------------------------
/**
*/
template<typename T, size_t N>
constexpr typename constexpr_array<T, N>::iterator
constexpr_array<T, N>::begin() noexcept
{
    return this->m_data;
}

//------------------------------------------------------------------------------
/**
*/
template<typename T, size_t N>
constexpr typename constexpr_array<T, N>::const_iterator
constexpr_array<T, N>::begin() const noexcept
{
    return this->m_data;
}

//------------------------------------------------------------------------------
/**
*/
template<typename T, size_t N>
constexpr typename constexpr_array<T, N>::const_iterator
constexpr_array<T, N>::cbegin() const noexcept
{
    return this->m_data;
}

//------------------------------------------------------------------------------
/**
*/
template<typename T, size_t N>
constexpr typename constexpr_array<T, N>::iterator
constexpr_array<T, N>::end() noexcept
{
    return this->m_data + N;
}

//------------------------------------------------------------------------------
/**
*/
template<typename T, size_t N>
constexpr typename constexpr_array<T, N>::const_iterator 
constexpr_array<T, N>::end() const noexcept
{
    return this->m_data + N;
}

//------------------------------------------------------------------------------
/**
*/
template<typename T, size_t N>
constexpr typename constexpr_array<T, N>::const_iterator
constexpr_array<T, N>::cend() const noexcept
{
    return this->m_data + N;
}

//------------------------------------------------------------------------------
/**
*/
template<typename T, size_t N>
constexpr typename constexpr_array<T, N>::reverse_iterator
constexpr_array<T, N>::rbegin() noexcept
{
    return reverse_iterator(this->end());
}

//------------------------------------------------------------------------------
/**
*/
template<typename T, size_t N>
constexpr typename constexpr_array<T, N>::const_reverse_iterator
constexpr_array<T, N>::rbegin() const noexcept
{
    return const_reverse_iterator(this->end());
}

//------------------------------------------------------------------------------
/**
*/
template<typename T, size_t N>
constexpr typename constexpr_array<T, N>::const_reverse_iterator
constexpr_array<T, N>::crbegin() const noexcept
{
    return const_reverse_iterator(this->end());
}

//------------------------------------------------------------------------------
/**
*/
template<typename T, size_t N>
constexpr typename constexpr_array<T, N>::reverse_iterator
constexpr_array<T, N>::rend() noexcept
{
    return reverse_iterator(this->begin());
}

//------------------------------------------------------------------------------
/**
*/
template<typename T, size_t N>
constexpr typename constexpr_array<T, N>::const_reverse_iterator
constexpr_array<T, N>::rend() const noexcept
{
    return const_reverse_iterator(this->begin());
}

//------------------------------------------------------------------------------
/**
*/
template<typename T, size_t N>
constexpr typename constexpr_array<T, N>::const_reverse_iterator
constexpr_array<T, N>::crend() const noexcept
{
    return const_reverse_iterator(this->begin());
}

//------------------------------------------------------------------------------
/**
*/
template<typename T, size_t N>
constexpr typename constexpr_array<T, N>::size_type
constexpr_array<T, N>::size() const
{
    return N;
}

//------------------------------------------------------------------------------
/**
*/
template<typename T, size_t N>
constexpr typename constexpr_array<T, N>::reference
constexpr_array<T, N>::operator[](size_type index)
{
    return this->m_data[index];
}

//------------------------------------------------------------------------------
/**
*/
template<typename T, size_t N>
constexpr typename constexpr_array<T, N>::const_reference
constexpr_array<T, N>::operator[](size_type index) const
{
    return this->m_data[index];
}

//------------------------------------------------------------------------------
/**
*/
template<typename T, size_t N>
constexpr typename constexpr_array<T, N>::reference
constexpr_array<T, N>::front()
{
    return this->m_data[0];
}

//------------------------------------------------------------------------------
/**
*/
template<typename T, size_t N>
constexpr typename constexpr_array<T, N>::const_reference
constexpr_array<T, N>::front() const
{
    return this->m_data[0];
}

//------------------------------------------------------------------------------
/**
*/
template<typename T, size_t N>
constexpr typename constexpr_array<T, N>::reference
constexpr_array<T, N>::back()
{
    return this->m_data[N - 1];
}

//------------------------------------------------------------------------------
/**
*/
template<typename T, size_t N>
constexpr typename constexpr_array<T, N>::const_reference
constexpr_array<T, N>::back() const
{
    return this->m_data[N - 1];
}

//------------------------------------------------------------------------------
/**
*/
template<typename T, size_t N>
constexpr typename constexpr_array<T, N>::reference
constexpr_array<T, N>::at(size_type index)
{
    return this->m_data[index];
}

//------------------------------------------------------------------------------
/**
*/
template<typename T, size_t N>
constexpr typename constexpr_array<T, N>::const_reference
constexpr_array<T, N>::at(size_type index) const
{
    return this->m_data[index];
}

//------------------------------------------------------------------------------
/**
*/
template<typename T, size_t N>
constexpr void 
constexpr_array<T, N>::fill(const value_type& val)
{
    for(size_type i = 0; i < N; ++i)
    {
        this->m_data[i] = val;
    }
}

} // namespace xr::etl::containers
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
namespace xr::etl::algorithms
{

template <typename T, std::size_t N, class Compare>
constexpr containers::constexpr_array<T, N> quicksort_r(
    containers::constexpr_array<T, N> const &array,
    Compare const &compare)
{
    containers::constexpr_array<T, N> res { array };
    quicksort(res.begin(), res.end() - 1, compare);
    return res;
}

} // namespace xr
//------------------------------------------------------------------------------