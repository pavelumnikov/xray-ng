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