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
class intrusive_list_node
{
public:
    using pointer = eastl::add_pointer_t<T>;
    using reference = eastl::add_lvalue_reference_t<T>;

    intrusive_list_node() noexcept;
    ~intrusive_list_node();

    XR_DECLARE_DEFAULT_COPY_ASSIGNMENT(intrusive_list_node);
    XR_DECLARE_DEFAULT_MOVE_ASSIGNMENT(intrusive_list_node);

    void append_before_this(reference node) noexcept;
    void append_after_this(reference node) noexcept;
    void remove_this() noexcept;

    bool is_head() const noexcept;
    bool has_next() const noexcept;
    bool has_prev() const noexcept;

    reference next() const noexcept;
    reference prev() const noexcept;

private:
    pointer m_next;
    pointer m_prev;
};

//------------------------------------------------------------------------------
/**
*/
template<typename T>
intrusive_list_node<T>::intrusive_list_node() noexcept
    : m_next { nullptr }
    , m_prev { nullptr }
{}

//------------------------------------------------------------------------------
/**
*/
template<typename T>
intrusive_list_node<T>::~intrusive_list_node()
{
    this->remove_this();
}

//------------------------------------------------------------------------------
/**
*/
template<typename T>
void intrusive_list_node<T>::append_before_this(reference node) noexcept
{
    if(this->has_prev()) // change previous node if exists
    {
        auto& p = this->prev();
        p.m_next = &node;
        node.m_prev = &p;
    }

    this->m_prev = &node;
    node.m_next = static_cast<pointer>(this);
}

//------------------------------------------------------------------------------
/**
*/
template<typename T>
void intrusive_list_node<T>::append_after_this(reference node) noexcept
{
    if(this->has_next()) // change next node if exists
    {
        auto& p = this->next();
        p.m_prev = &node;
        node.m_next = &p;
    }

    this->m_next = &node;
    node.m_prev = static_cast<pointer>(this);
}

//------------------------------------------------------------------------------
/**
*/
template<typename T>
void intrusive_list_node<T>::remove_this() noexcept
{
    if(this->m_prev)
    {
        this->m_prev->m_next = this->m_next;
    }

    if(this->m_next)
    {
        this->m_next->m_prev = this->m_prev;
    }
}

//------------------------------------------------------------------------------
/**
*/
template<typename T>
bool intrusive_list_node<T>::is_head() const noexcept
{
    return this->m_prev == nullptr;
}

//------------------------------------------------------------------------------
/**
*/
template <typename T>
bool intrusive_list_node<T>::has_next() const noexcept
{
    return this->m_next != nullptr;
}

//------------------------------------------------------------------------------
/**
*/
template <typename T>
bool intrusive_list_node<T>::has_prev() const noexcept
{
    return this->m_prev != nullptr;
}

//------------------------------------------------------------------------------
/**
*/
template<typename T>
typename intrusive_list_node<T>::reference
intrusive_list_node<T>::next() const noexcept
{
    XR_DEBUG_ASSERTION(this->has_next());
    return *this->m_next;
}

//------------------------------------------------------------------------------
/**
*/
template <typename T>
typename intrusive_list_node<T>::reference
intrusive_list_node<T>::prev() const noexcept
{
    XR_DEBUG_ASSERTION(this->has_prev());
    return *this->m_prev;
}


} // namespace xr::etl::containers
//------------------------------------------------------------------------------