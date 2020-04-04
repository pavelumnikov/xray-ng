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

//------------------------------------------------------------------------------
namespace xr::etl::containers
{

//------------------------------------------------------------------------------
template<typename T>
class hierarchy_node
{
    static_assert(eastl::is_base_of_v<hierarchy_node<T>, T>);
    static_assert(eastl::is_convertible_v<T, hierarchy_node<T>>);

public:
    using pointer = eastl::add_pointer_t<T>;
    using reference = eastl::add_lvalue_reference_t<T>;
    using const_reference = eastl::add_lvalue_reference_t<const T>;

    hierarchy_node() noexcept;
    ~hierarchy_node();

    XR_DECLARE_DELETE_COPY_ASSIGNMENT(hierarchy_node);
    XR_DECLARE_DEFAULT_MOVE_ASSIGNMENT(hierarchy_node);

    void parent_to(reference parent) noexcept;
    void append_sibling_after(reference n) noexcept;

    bool has_parent() const noexcept;
    bool has_child() const noexcept;
    bool has_sibling() const noexcept;

    bool is_parent_to(const_reference n) const noexcept;
    bool is_parented_by(const_reference n) const noexcept;

    void remove_from_parent() noexcept;
    void remove_from_hierarchy() noexcept;

    // parent of this node
    reference parent() const noexcept;
    // first child of this node
    reference child() const noexcept;
    // next node with the same parent
    pointer next_sibling() const noexcept;
    // previous node with the same parent
    pointer prev_sibling() const noexcept;

private:
    pointer m_parent;
    pointer m_sibling;
    pointer m_child;
}; // class hierarchy_node


//------------------------------------------------------------------------------
/**
*/
template<typename T>
hierarchy_node<T>::hierarchy_node() noexcept
    : m_parent { nullptr }
    , m_sibling { nullptr }
    , m_child { nullptr }
{}

//------------------------------------------------------------------------------
/**
*/
template<typename T>
hierarchy_node<T>::~hierarchy_node()
{
    this->remove_from_hierarchy();
}

//------------------------------------------------------------------------------
/**
*/
template<typename T>
void hierarchy_node<T>::parent_to(reference parent) noexcept
{
    this->remove_from_parent();

    this->m_parent = &parent;
    this->m_sibling = parent.m_child;
    parent.m_child = static_cast<pointer>(this);
}

//------------------------------------------------------------------------------
/**
*/
template<typename T>
void hierarchy_node<T>::append_sibling_after(reference n) noexcept
{
    this->remove_from_parent();

    this->m_parent = n.m_parent;
    this->m_sibling = n.m_sibling;
    n.m_sibling = static_cast<pointer>(this);
}

//------------------------------------------------------------------------------
/**
*/
template<typename T>
bool hierarchy_node<T>::has_parent() const noexcept
{
    return this->m_parent != nullptr;
}

//------------------------------------------------------------------------------
/**
*/
template<typename T>
bool hierarchy_node<T>::has_child() const noexcept
{
    return this->m_child != nullptr;
}

//------------------------------------------------------------------------------
/**
*/
template <typename T>
bool hierarchy_node<T>::has_sibling() const noexcept
{
    return this->m_sibling != nullptr;
}

//------------------------------------------------------------------------------
/**
*/
template<typename T>
bool hierarchy_node<T>::is_parent_to(const_reference n) const noexcept
{
    return n.m_parent == static_cast<pointer>(this);
}

//------------------------------------------------------------------------------
/**
*/
template<typename T>
bool hierarchy_node<T>::is_parented_by(const_reference n) const noexcept
{
    return this->m_parent == &n;
}

//------------------------------------------------------------------------------
/**
*/
template<typename T>
void hierarchy_node<T>::remove_from_parent() noexcept
{
    pointer prev { nullptr };

    if(this->parent())
    {
        prev = this->prev_sibling();
        if(prev)
            prev->m_sibling = this->m_sibling;
        else
            parent->m_child = this->m_sibling;
    }

    this->m_parent = nullptr;
    this->m_sibling = nullptr;
}

//------------------------------------------------------------------------------
/**
*/
template<typename T>
void hierarchy_node<T>::remove_from_hierarchy() noexcept
{
    pointer parent_node { nullptr }, node { nullptr };

    parent_node = this->parent();
    this->remove_from_parent();

    if(parent_node)
    {
        for(; this->has_child(); node = this->child())
        {
            node->remove_from_parent();
            node->parent_to(*parent_node);
        }
    }
    else
    {
        while(this->has_child())
            this->m_child->remove_from_parent();
    }
}

//------------------------------------------------------------------------------
/**
*/
template<typename T>
typename hierarchy_node<T>::reference
hierarchy_node<T>::parent() const noexcept
{
    XR_DEBUG_ASSERTION(this->has_parent());
    return *this->m_parent;
}

//------------------------------------------------------------------------------
/**
*/
template<typename T>
typename hierarchy_node<T>::reference
hierarchy_node<T>::child() const noexcept
{
    XR_DEBUG_ASSERTION(this->has_child());
    return *this->m_child;
}

//------------------------------------------------------------------------------
/**
*/
template<typename T>
typename hierarchy_node<T>::pointer
hierarchy_node<T>::next_sibling() const noexcept
{
    return this->m_sibling;
}

//------------------------------------------------------------------------------
/**
*/
template<typename T>
typename hierarchy_node<T>::pointer
hierarchy_node<T>::prev_sibling() const noexcept
{
    if(!this->parent() || (this->parent()->child() == static_cast<pointer>(this)))
        return nullptr;

    pointer prev { nullptr }, node { this->parent()->child() };
    while((node != static_cast<pointer>(this)) && (node != nullptr))
    {
        prev = node;
        node = node->m_sibling;
    }

    XR_DEBUG_ASSERTION_MSG(node == static_cast<pointer>(this),
        "hierarchy_node<T>::prev_sibling: could not find node in parent's list of children");

    return prev;
}

} // namespace xr::etl::containers
//------------------------------------------------------------------------------