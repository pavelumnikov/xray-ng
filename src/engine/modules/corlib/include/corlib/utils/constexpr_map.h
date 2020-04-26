// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/utils/details/compare_key.h"
#include "corlib/utils/details/constexpr_quicksort.h"
#include "corlib/utils/details/constexpr_binarysearch.h"
#include "EASTL/array.h"

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, utils)

//-----------------------------------------------------------------------------------------------------------
template <class Key, class Value, size_t N, class Compare = eastl::less<Key>>
class constexpr_map
{
    static_assert(N > 0, "map must have at lest one element");

    using container_pair = pair<Key, Value>;
    using container_type = eastl::array<container_pair, N>;
    using container_comparer = details::compare_key<Compare>;

public:
    using key_type = Key;
    using mapped_type = Value;
    using value_type = typename container_type::value_type;
    using size_type = typename container_type::size_type;
    using difference_type = typename container_type::difference_type;
    using key_compare = container_comparer;
    using const_reference = typename container_type::const_reference;
    using reference = const_reference;
    using const_pointer = typename container_type::const_pointer;
    using pointer = const_pointer;
    using const_iterator = typename container_type::const_iterator;
    using iterator = const_iterator;
    using const_reverse_iterator = typename container_type::const_reverse_iterator;
    using reverse_iterator = const_reverse_iterator;
    using equal_range_pair = pair<const_iterator, const_iterator>;

    XR_CONSTEXPR_CPP14_OR_INLINE constexpr_map(container_type items, Compare const &compare);
    explicit XR_CONSTEXPR_CPP14_OR_INLINE constexpr_map(container_type items);
    XR_CONSTEXPR_CPP14_OR_INLINE constexpr_map(std::initializer_list<value_type> items, Compare const &compare);
    XR_CONSTEXPR_CPP14_OR_INLINE constexpr_map(std::initializer_list<value_type> items);

    // Element access

    XR_CONSTEXPR_CPP14_OR_INLINE mapped_type at(Key const &key) const;

    // Iterators

    XR_CONSTEXPR_CPP14_OR_INLINE const_iterator begin() const;
    XR_CONSTEXPR_CPP14_OR_INLINE const_iterator cbegin() const;
    XR_CONSTEXPR_CPP14_OR_INLINE const_iterator end() const;
    XR_CONSTEXPR_CPP14_OR_INLINE const_iterator cend() const;

    XR_CONSTEXPR_CPP14_OR_INLINE const_reverse_iterator rbegin() const;
    XR_CONSTEXPR_CPP14_OR_INLINE const_reverse_iterator crbegin() const;
    XR_CONSTEXPR_CPP14_OR_INLINE const_reverse_iterator rend() const;
    XR_CONSTEXPR_CPP14_OR_INLINE const_reverse_iterator crend() const;

    // Capacity

    XR_CONSTEXPR_CPP14_OR_INLINE bool empty() const;
    XR_CONSTEXPR_CPP14_OR_INLINE size_type size() const;
    XR_CONSTEXPR_CPP14_OR_INLINE size_type max_size() const;

    // Lookup

    XR_CONSTEXPR_CPP14_OR_INLINE size_type count(Key const &key) const;
    XR_CONSTEXPR_CPP14_OR_INLINE const_iterator find(Key const &key) const;
    XR_CONSTEXPR_CPP14_OR_INLINE equal_range_pair equal_range(Key const &key) const;
    XR_CONSTEXPR_CPP14_OR_INLINE const_iterator lower_bound(Key const &key) const;
    XR_CONSTEXPR_CPP14_OR_INLINE const_iterator upper_bound(Key const &key) const;

private:
    container_type m_items;
    container_comparer m_comparer;
};

//-----------------------------------------------------------------------------------------------------------
/**
*/
template <typename T, typename U, size_t N>
XR_CONSTEXPR_CPP14_OR_INLINE auto make_constexpr_map(pair<T, U> const (&items)[N])
{
    return constexpr_map<T, U, N>(items);
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
template <typename T, typename U, size_t N, typename Comp>
XR_CONSTEXPR_CPP14_OR_INLINE auto make_constexpr_map(pair<T, U> const (&items)[N], Comp const &compare)
{
    return constexpr_map<T, U, N, Comp>(items, compare);
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
template <typename Key, typename Value, size_t N, typename Compare>
XR_CONSTEXPR_CPP14_OR_INLINE 
constexpr_map<Key, Value, N, Compare>::constexpr_map(container_type items, Compare const &compare)
    : m_comparer(compare)
    , m_items(quicksort_r(items, m_comparer))
{}

//-----------------------------------------------------------------------------------------------------------
/**
*/
template <typename Key, typename Value, size_t N, typename Compare>
XR_CONSTEXPR_CPP14_OR_INLINE 
constexpr_map<Key, Value, N, Compare>::constexpr_map(container_type items)
    : m_comparer(Compare())
    , m_items(quicksort_r(items, m_comparer))
{}

//-----------------------------------------------------------------------------------------------------------
/**
*/
template <typename Key, typename Value, size_t N, typename Compare>
XR_CONSTEXPR_CPP14_OR_INLINE 
constexpr_map<Key, Value, N, Compare>::constexpr_map(std::initializer_list<value_type> items, 
    Compare const &compare)
    : constexpr_map { container_type { items }, compare }
{
#if defined(_MSC_VER)
    // clang & gcc doesn't recognize items.size() as a XR_CONSTEXPR_CPP14_OR_INLINE
    static_assert(items.size() == N, "inconsistent initializer_list size and type size argument");
#endif // defined(_MSC_VER)
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
template <typename Key, typename Value, size_t N, typename Compare>
XR_CONSTEXPR_CPP14_OR_INLINE 
constexpr_map<Key, Value, N, Compare>::constexpr_map(
    std::initializer_list<value_type> items)
    : constexpr_map { items, Compare {} }
{}

//-----------------------------------------------------------------------------------------------------------
/**
*/
template <typename Key, typename Value, size_t N, typename Compare>
XR_CONSTEXPR_CPP14_OR_INLINE typename constexpr_map<Key, Value, N, Compare>::mapped_type 
constexpr_map<Key, Value, N, Compare>::at(Key const &key) const
{
    auto const where = lower_bound(key);
    if(where != end())
        return where->second;
    else
        std::abort();
}


//-----------------------------------------------------------------------------------------------------------
/**
*/
template <typename Key, typename Value, size_t N, typename Compare>
XR_CONSTEXPR_CPP14_OR_INLINE typename constexpr_map<Key, Value, N, Compare>::const_iterator 
constexpr_map<Key, Value, N, Compare>::begin() const
{
    return m_items.begin();
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
template <typename Key, typename Value, size_t N, typename Compare>
XR_CONSTEXPR_CPP14_OR_INLINE typename constexpr_map<Key, Value, N, Compare>::const_iterator 
constexpr_map<Key, Value, N, Compare>::cbegin() const
{
    return m_items.cbegin();
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
template <typename Key, typename Value, size_t N, typename Compare>
XR_CONSTEXPR_CPP14_OR_INLINE typename constexpr_map<Key, Value, N, Compare>::const_iterator 
constexpr_map<Key, Value, N, Compare>::end() const
{
    return m_items.end();
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
template <typename Key, typename Value, size_t N, typename Compare>
XR_CONSTEXPR_CPP14_OR_INLINE typename constexpr_map<Key, Value, N, Compare>::const_iterator 
constexpr_map<Key, Value, N, Compare>::cend() const
{
    return m_items.cend();
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
template <typename Key, typename Value, size_t N, typename Compare>
XR_CONSTEXPR_CPP14_OR_INLINE typename constexpr_map<Key, Value, N, Compare>::const_reverse_iterator 
constexpr_map<Key, Value, N, Compare>::rbegin() const
{
    return m_items.rbegin();
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
template <typename Key, typename Value, size_t N, typename Compare>
XR_CONSTEXPR_CPP14_OR_INLINE typename constexpr_map<Key, Value, N, Compare>::const_reverse_iterator 
constexpr_map<Key, Value, N, Compare>::crbegin() const
{
    return m_items.crbegin();
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
template <typename Key, typename Value, size_t N, typename Compare>
XR_CONSTEXPR_CPP14_OR_INLINE typename constexpr_map<Key, Value, N, Compare>::const_reverse_iterator 
constexpr_map<Key, Value, N, Compare>::rend() const
{
    return m_items.rend();
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
template <typename Key, typename Value, size_t N, typename Compare>
XR_CONSTEXPR_CPP14_OR_INLINE typename constexpr_map<Key, Value, N, Compare>::const_reverse_iterator 
constexpr_map<Key, Value, N, Compare>::crend() const
{
    return m_items.crend();
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
template <typename Key, typename Value, size_t N, typename Compare>
XR_CONSTEXPR_CPP14_OR_INLINE bool 
constexpr_map<Key, Value, N, Compare>::empty() const
{
    return !N;
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
template <typename Key, typename Value, size_t N, typename Compare>
XR_CONSTEXPR_CPP14_OR_INLINE typename constexpr_map<Key, Value, N, Compare>::size_type 
constexpr_map<Key, Value, N, Compare>::size() const
{
    return N;
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
template <typename Key, typename Value, size_t N, typename Compare>
XR_CONSTEXPR_CPP14_OR_INLINE typename constexpr_map<Key, Value, N, Compare>::size_type 
constexpr_map<Key, Value, N, Compare>::max_size() const
{
    return N;
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
template <typename Key, typename Value, size_t N, typename Compare>
XR_CONSTEXPR_CPP14_OR_INLINE typename constexpr_map<Key, Value, N, Compare>::size_type 
constexpr_map<Key, Value, N, Compare>::count(Key const &key) const
{
    return details::binary_search<N>(m_items.begin(), key, m_comparer);
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
template <typename Key, typename Value, size_t N, typename Compare>
XR_CONSTEXPR_CPP14_OR_INLINE typename constexpr_map<Key, Value, N, Compare>::const_iterator
constexpr_map<Key, Value, N, Compare>::find(Key const &key) const
{
    const_iterator where = lower_bound(key);
    if((where != end()) && !m_comparer(key, *where))
        return where;
    else
        return end();
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
template <typename Key, typename Value, size_t N, typename Compare>
XR_CONSTEXPR_CPP14_OR_INLINE typename constexpr_map<Key, Value, N, Compare>::equal_range_pair
constexpr_map<Key, Value, N, Compare>::equal_range(Key const &key) const
{
    auto const lower = lower_bound(key);
    if(lower == end())
        return { lower, lower };
    else
        return { lower, lower + 1 };
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
template <typename Key, typename Value, size_t N, typename Compare>
XR_CONSTEXPR_CPP14_OR_INLINE typename constexpr_map<Key, Value, N, Compare>::const_iterator
constexpr_map<Key, Value, N, Compare>::lower_bound(Key const &key) const
{
    auto const where = details::lower_bound<N>(m_items.begin(), key, m_comparer);
    if((where != end()) && !this->m_comparer(key, *where))
        return where;
    else
        return end();
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
template <typename Key, typename Value, size_t N, typename Compare>
XR_CONSTEXPR_CPP14_OR_INLINE typename constexpr_map<Key, Value, N, Compare>::const_iterator
constexpr_map<Key, Value, N, Compare>::upper_bound(Key const &key) const
{
    auto const where = details::lower_bound<N>(m_items.begin(), key, m_comparer);
    if((where != end()) && !less_than_(key, *where))
        return where + 1;
    else
        return end();
}

XR_NAMESPACE_END(xr, utils)
//-----------------------------------------------------------------------------------------------------------
