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

#include "corlib/etl/containers/associative_vector_compare_predicate.h" // for associative_vector_compare_predicate
#include "EASTL/algorithm.h" // for eastl::lower_bound

namespace xr::etl::containers
{

template <
    typename key_type,
    typename data_type,
    template <typename T> class container_type,
    typename compare_predicate_type = eastl::less< key_type >
>
class associative_vector
    : protected container_type<std::pair<key_type, data_type>>
    , protected associative_vector_compare_predicate<key_type, data_type, compare_predicate_type>
{
private:
    using self_type = associative_vector<key_type, data_type, container_type, compare_predicate_type>;
    using super = container_type<std::pair<key_type, data_type>>;

public:
    using value_compare = associative_vector_compare_predicate<key_type, data_type, compare_predicate_type>;
    using allocator_type = typename super::allocator_type;
    using const_pointer = typename super::const_pointer;
    using const_reference = typename super::const_reference;
    using const_iterator = typename super::const_iterator;
    using const_reverse_iterator = typename super::const_reverse_iterator;
    using pointer = typename super::pointer;
    using reference = typename super::reference;
    using iterator = typename super::iterator;
    using reverse_iterator = typename super::reverse_iterator;
    using difference_type = typename allocator_type::difference_type;
    using key_compare = compare_predicate_type;
    using mapped_type = data_type;
    using size_type = typename super::size_type;
    using value_type = typename super::value_type;
    using insert_result = eastl::pair<iterator, bool>;
    using equal_range_result = eastl::pair<iterator, iterator>;
    using const_equal_range_result = eastl::pair<const_iterator, const_iterator>;

public:
    template <typename iterator_type>
    associative_vector(iterator_type first, iterator_type last, 
        key_compare const& predicate = key_compare(), allocator_type const& allocator = allocator_type());

    associative_vector(key_compare const& predicate = key_compare(), 
        allocator_type const& allocator = allocator_type());

    explicit associative_vector(key_compare const& predicate);

    mapped_type& operator[](key_type const& key);

    bool operator < (self_type const& right) const;
    bool operator <= (self_type const& right) const;
    bool operator > (self_type const& right) const;
    bool operator >= (self_type const& right) const;
    bool operator == (self_type const& right) const;
    bool operator != (self_type const& right) const;

    iterator begin();
    iterator end();
    reverse_iterator rbegin();
    iterator rend();

    insert_result insert(value_type const& value);
    iterator insert(iterator where, value_type const& value);

    template <class iterator_type>
    void insert(iterator_type first, iterator_type last);

    void erase(iterator element);
    void erase(iterator first, iterator last);
    size_type erase(key_type const& key);

    void clear();
    iterator find(key_type const& key);
    iterator lower_bound(key_type const& key);
    iterator upper_bound(key_type const& key);
    equal_range_result equal_range(key_type const& key);
    void swap(self_type& object);

    const_iterator begin() const;
    const_iterator end() const;
    const_reverse_iterator rbegin() const;
    const_reverse_iterator rend() const;
    const_iterator find(key_type const& key) const;
    const_iterator lower_bound(key_type const& key) const;
    const_iterator upper_bound(key_type const& key) const;
    const_equal_range_result equal_range(key_type const& key) const;
    size_type count(key_type const& key) const;
    size_type max_size() const;
    size_type size() const;
    bool empty() const;
    key_compare key_comp() const;
    value_compare value_comp() const;

    allocator_type& get_allocator() const;

private:
    void actualize() const;
}; // class associative_vector

template <
    typename key_type,
    typename data_type,
    template <typename T> class container_type,
    typename compare_predicate_type
>
inline void swap(associative_vector<key_type, data_type, container_type, compare_predicate_type>& left,
    associative_vector<key_type, data_type, container_type, compare_predicate_type>& right)
{
    left.swap(right);
}

} // namespace xr::etl::containers
