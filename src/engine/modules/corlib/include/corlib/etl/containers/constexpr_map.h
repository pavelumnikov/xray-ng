// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/etl/containers/constexpr_array.h"
#include "corlib/etl/algorithms/constexpr_quicksort.h"
#include "corlib/etl/algorithms/constexpr_binarysearch.h"

//------------------------------------------------------------------------------
namespace xr::etl::containers::details
{

//------------------------------------------------------------------------------
template<class Comparer> 
class compare_key
{
    using comparer_type = Comparer;
    comparer_type const m_comparer;

public:
    constexpr compare_key(comparer_type const &comparer)
        : m_comparer(comparer)
    {}

    template <class Key, class Value>
    constexpr int operator()(eastl::pair<Key, Value> const &self,
        eastl::pair<Key, Value> const &other) const
    {
        return m_comparer(eastl::get<0>(self), eastl::get<0>(other));
    }

    template <class Key, class Value>
    constexpr int operator()(Key const &self_key,
        eastl::pair<Key, Value> const &other) const
    {
        return m_comparer(self_key, ::eastl::get<0>(other));
    }

    template <class Key, class Value>
    constexpr int operator()(::eastl::pair<Key, Value> const &self,
        Key const &other_key) const
    {
        return m_comparer(eastl::get<0>(self), other_key);
    }

    template <class Key>
    constexpr int operator()(Key const &self_key, Key const &other_key) const
    {
        return m_comparer(self_key, other_key);
    }
}; // class compare_key

} // namespace xr::etl::containers::details
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
namespace xr::etl::containers
{

//------------------------------------------------------------------------------
template <class Key, class Value, size_t N, class Compare = eastl::less<Key>>
class constexpr_map
{
    static_assert(N > 0, "map must have at lest one element");

    using container_pair = ::std::pair<Key, Value>;
    using container_type = containers::constexpr_array<container_pair, N>;
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
    using const_reverse_iterator =
        typename container_type::const_reverse_iterator;
    using reverse_iterator = const_reverse_iterator;
    using equal_range_pair = eastl::pair<const_iterator, const_iterator>;

    constexpr constexpr_map(container_type items, Compare const &compare);
    explicit constexpr constexpr_map(container_type items);
    constexpr constexpr_map(std::initializer_list<value_type> items, Compare const &compare);
    constexpr constexpr_map(std::initializer_list<value_type> items);

    // Element access

    constexpr mapped_type at(Key const &key) const;

    // Iterators

    constexpr const_iterator begin() const;
    constexpr const_iterator cbegin() const;
    constexpr const_iterator end() const;
    constexpr const_iterator cend() const;

    constexpr const_reverse_iterator rbegin() const;
    constexpr const_reverse_iterator crbegin() const;
    constexpr const_reverse_iterator rend() const;
    constexpr const_reverse_iterator crend() const;

    // Capacity

    constexpr bool empty() const;
    constexpr size_type size() const;
    constexpr size_type max_size() const;

    // Lookup

    constexpr size_type count(Key const &key) const;
    constexpr const_iterator find(Key const &key) const;
    constexpr equal_range_pair equal_range(Key const &key) const;
    constexpr const_iterator lower_bound(Key const &key) const;
    constexpr const_iterator upper_bound(Key const &key) const;

private:
    container_type m_items;
    container_comparer m_comparer;
};

//------------------------------------------------------------------------------
/**
*/
template <typename T, typename U, size_t N>
constexpr auto make_constexpr_map(eastl::pair<T, U> const (&items)[N])
{
    return constexpr_map<T, U, N>{ items };
}

//------------------------------------------------------------------------------
/**
*/
template <typename T, typename U, size_t N, typename Comp>
constexpr auto make_constexpr_map(eastl::pair<T, U> const (&items)[N], Comp const &compare)
{
    return constexpr_map<T, U, N, Comp>{ items, compare };
}

//------------------------------------------------------------------------------
/**
*/
template <typename Key, typename Value, size_t N, typename Compare>
constexpr 
constexpr_map<Key, Value, N, Compare>::constexpr_map(container_type items, Compare const &compare)
    : m_comparer { compare }
    , m_items { algorithms::quicksort_r(items, m_comparer) } {}

//------------------------------------------------------------------------------
/**
*/
template <typename Key, typename Value, size_t N, typename Compare>
constexpr 
constexpr_map<Key, Value, N, Compare>::constexpr_map(container_type items)
    : constexpr_map { items, Compare {} }
{}

//------------------------------------------------------------------------------
/**
*/
template <typename Key, typename Value, size_t N, typename Compare>
constexpr 
constexpr_map<Key, Value, N, Compare>::constexpr_map(::std::initializer_list<value_type> items, 
    Compare const &compare)
    : constexpr_map { container_type { items }, compare }
{
#if defined(_MSC_VER)
    // clang & gcc doesn't recognize items.size() as a constexpr
    static_assert(items.size() == N, "inconsistent initializer_list size and type size argument");
#endif // defined(_MSC_VER)
}

//------------------------------------------------------------------------------
/**
*/
template <typename Key, typename Value, size_t N, typename Compare>
constexpr 
constexpr_map<Key, Value, N, Compare>::constexpr_map(
    std::initializer_list<value_type> items)
    : constexpr_map { items, Compare {} }
{}

//------------------------------------------------------------------------------
/**
*/
template <typename Key, typename Value, size_t N, typename Compare>
constexpr typename constexpr_map<Key, Value, N, Compare>::mapped_type 
constexpr_map<Key, Value, N, Compare>::at(Key const &key) const
{
    auto const where = lower_bound(key);
    if(where != end())
        return where->second;
    else
        std::abort();
}


//------------------------------------------------------------------------------
/**
*/
template <typename Key, typename Value, size_t N, typename Compare>
constexpr typename constexpr_map<Key, Value, N, Compare>::const_iterator 
constexpr_map<Key, Value, N, Compare>::begin() const
{
    return m_items.begin();
}

//------------------------------------------------------------------------------
/**
*/
template <typename Key, typename Value, size_t N, typename Compare>
constexpr typename constexpr_map<Key, Value, N, Compare>::const_iterator 
constexpr_map<Key, Value, N, Compare>::cbegin() const
{
    return m_items.cbegin();
}

//------------------------------------------------------------------------------
/**
*/
template <typename Key, typename Value, size_t N, typename Compare>
constexpr typename constexpr_map<Key, Value, N, Compare>::const_iterator 
constexpr_map<Key, Value, N, Compare>::end() const
{
    return m_items.end();
}

//------------------------------------------------------------------------------
/**
*/
template <typename Key, typename Value, size_t N, typename Compare>
constexpr typename constexpr_map<Key, Value, N, Compare>::const_iterator 
constexpr_map<Key, Value, N, Compare>::cend() const
{
    return m_items.cend();
}

//------------------------------------------------------------------------------
/**
*/
template <typename Key, typename Value, size_t N, typename Compare>
constexpr typename constexpr_map<Key, Value, N, Compare>::const_reverse_iterator 
constexpr_map<Key, Value, N, Compare>::rbegin() const
{
    return m_items.rbegin();
}

//------------------------------------------------------------------------------
/**
*/
template <typename Key, typename Value, size_t N, typename Compare>
constexpr typename constexpr_map<Key, Value, N, Compare>::const_reverse_iterator 
constexpr_map<Key, Value, N, Compare>::crbegin() const
{
    return m_items.crbegin();
}

//------------------------------------------------------------------------------
/**
*/
template <typename Key, typename Value, size_t N, typename Compare>
constexpr typename constexpr_map<Key, Value, N, Compare>::const_reverse_iterator 
constexpr_map<Key, Value, N, Compare>::rend() const
{
    return m_items.rend();
}

//------------------------------------------------------------------------------
/**
*/
template <typename Key, typename Value, size_t N, typename Compare>
constexpr typename constexpr_map<Key, Value, N, Compare>::const_reverse_iterator 
constexpr_map<Key, Value, N, Compare>::crend() const
{
    return m_items.crend();
}

//------------------------------------------------------------------------------
/**
*/
template <typename Key, typename Value, size_t N, typename Compare>
constexpr bool 
constexpr_map<Key, Value, N, Compare>::empty() const
{
    return !N;
}

//------------------------------------------------------------------------------
/**
*/
template <typename Key, typename Value, size_t N, typename Compare>
constexpr typename constexpr_map<Key, Value, N, Compare>::size_type 
constexpr_map<Key, Value, N, Compare>::size() const
{
    return N;
}

//------------------------------------------------------------------------------
/**
*/
template <typename Key, typename Value, size_t N, typename Compare>
constexpr typename constexpr_map<Key, Value, N, Compare>::size_type 
constexpr_map<Key, Value, N, Compare>::max_size() const
{
    return N;
}

//------------------------------------------------------------------------------
/**
*/
template <typename Key, typename Value, size_t N, typename Compare>
constexpr typename constexpr_map<Key, Value, N, Compare>::size_type 
constexpr_map<Key, Value, N, Compare>::count(Key const &key) const
{
    return algorithms::binary_search<N>(m_items.begin(), key, m_comparer);
}

//------------------------------------------------------------------------------
/**
*/
template <typename Key, typename Value, size_t N, typename Compare>
constexpr typename constexpr_map<Key, Value, N, Compare>::const_iterator
constexpr_map<Key, Value, N, Compare>::find(Key const &key) const
{
    const_iterator where = lower_bound(key);
    if((where != end()) && !m_comparer(key, *where))
        return where;
    else
        return end();
}

//------------------------------------------------------------------------------
/**
*/
template <typename Key, typename Value, size_t N, typename Compare>
constexpr typename constexpr_map<Key, Value, N, Compare>::equal_range_pair
constexpr_map<Key, Value, N, Compare>::equal_range(Key const &key) const
{
    auto const lower = lower_bound(key);
    if(lower == end())
        return { lower, lower };
    else
        return { lower, lower + 1 };
}

//------------------------------------------------------------------------------
/**
*/
template <typename Key, typename Value, size_t N, typename Compare>
constexpr typename constexpr_map<Key, Value, N, Compare>::const_iterator
constexpr_map<Key, Value, N, Compare>::lower_bound(Key const &key) const
{
    auto const where = algorithms::lower_bound<N>(m_items.begin(), key, m_comparer);
    if((where != end()) && !this->m_comparer(key, *where))
        return where;
    else
        return end();
}

//------------------------------------------------------------------------------
/**
*/
template <typename Key, typename Value, size_t N, typename Compare>
constexpr typename constexpr_map<Key, Value, N, Compare>::const_iterator
constexpr_map<Key, Value, N, Compare>::upper_bound(Key const &key) const
{
    auto const where = algorithms::lower_bound<N>(m_items.begin(), key, m_comparer);
    if((where != end()) && !less_than_(key, *where))
        return where + 1;
    else
        return end();
}

} // namespace xr::etl::containers
//------------------------------------------------------------------------------