// This file is a part of xray-ng engine
//

#pragma once

#include "EASTL/utility.h" // for eastl::pair

//------------------------------------------------------------------------------
namespace xr::etl::containers
{

//------------------------------------------------------------------------------
template <typename Key, typename Data, typename ComparePredicateType>
class associative_vector_compare_predicate : public ComparePredicateType
{
private:
    using super = ComparePredicateType;

public:
    using key_type = Key;
    using data_type = Data;
    using value_type = eastl::pair< key_type, data_type >;
    using compare_predicate_type = ComparePredicateType;

public:
    associative_vector_compare_predicate();
    explicit associative_vector_compare_predicate(compare_predicate_type const& compare_predicate);
    bool operator()(key_type const& lhs, key_type const& rhs) const;
    bool operator()(value_type const& lhs, value_type const& rhs) const;
    bool operator()(value_type const& lhs, key_type const& rhs) const;
    bool operator()(key_type const& lhs, value_type const& rhs) const;
}; // class associative_vector_compare_predicate

//------------------------------------------------------------------------------
/**
*/
template <typename Key, typename Data, typename ComparePredicateType>
associative_vector_compare_predicate<Key, Data, ComparePredicateType>::associative_vector_compare_predicate()
{}

//------------------------------------------------------------------------------
/**
*/
template <typename Key, typename Data, typename ComparePredicateType>
associative_vector_compare_predicate<Key, Data, ComparePredicateType>::associative_vector_compare_predicate(compare_predicate_type const& compare_predicate)
    : super(compare_predicate)
{}

//------------------------------------------------------------------------------
/**
*/
template <typename Key, typename Data, typename ComparePredicateType>
inline bool
associative_vector_compare_predicate<Key, Data, ComparePredicateType>::operator()(key_type const& lhs, key_type const& rhs) const
{
    return (super::operator()(lhs, rhs));
}

//------------------------------------------------------------------------------
/**
*/
template <typename Key, typename Data, typename ComparePredicateType>
inline bool
associative_vector_compare_predicate<Key, Data, ComparePredicateType>::operator()(value_type const& lhs, value_type const& rhs) const
{
    return (operator( ) (lhs.first, rhs.first));
}

//------------------------------------------------------------------------------
/**
*/
template <typename Key, typename Data, typename ComparePredicateType>
inline bool
associative_vector_compare_predicate<Key, Data, ComparePredicateType>::operator()(value_type const& lhs, key_type const& rhs) const
{
    return (operator( ) (lhs.first, rhs));
}

//------------------------------------------------------------------------------
/**
*/
template <typename Key, typename Data, typename ComparePredicateType>
inline bool
associative_vector_compare_predicate<Key, Data, ComparePredicateType>::operator()(key_type const& lhs, value_type const& rhs) const
{
    return (operator( ) (lhs, rhs.first));
}

} // namespace xr::etl::containers
//------------------------------------------------------------------------------