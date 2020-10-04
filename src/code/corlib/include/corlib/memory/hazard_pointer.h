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

#include "corlib/threading/interlocked.h"
#include <memory>

//------------------------------------------------------------------------------
namespace az::corlib::memory
{

// Forward declarations.

template <typename, typename>
class hazard_pointer_list;

//------------------------------------------------------------------------------
// Represents a shared list of hazard pointers.
template <typename T>
class hazard_pointer
{
public:
    using value_type = T;
    using pointer = std::add_pointer_t<value_type>;

public:
    constexpr hazard_pointer() noexcept;

    // Update the hazard pointer entry with a pointer value.
    void remember(pointer ptr);

    // Release a hazard pointer entry.
    void release();

private:
    template<typename Alloc>
    friend struct hazard_pointer_list<T, Alloc>;

    using hazard_pointer_t = std::add_pointer_t<hazard_pointer<T>>;

    // Next hazard pointer in the list
    hazard_pointer_t m_next;

    // Is this hazard pointer active? I.e. should the pointer
    // contained be considered as one that is held by another thread.
    volatile bool m_is_active;

    // The pointer held by this hazard pointer structure.
    volatile pointer m_hazardous_pointer;
}; // class hazard_pointer

//------------------------------------------------------------------------------
/**
*/
template <typename T>
constexpr
hazard_pointer<T>::hazard_pointer()
    : m_next(nullptr)
    , m_is_active(true)
    , m_hazardous_pointer(nullptr)
{}

//------------------------------------------------------------------------------
/**
*/
template <typename T>
inline void 
hazard_pointer<T>::remember(pointer ptr) throw()
{
    threading::atomic_store_seq(this->m_hazardous_pointer, ptr);
}

//------------------------------------------------------------------------------
/**
*/
template <typename T>
inline void 
hazard_pointer<T>::release(void) throw()
{
    AZ_MEMORY_FULLCONSISTENCY_BARRIER;
    threading::atomic_store_relax(this->m_hazardous_pointer, nullptr);
    threading::atomic_store_relax(this->m_is_active, false);
}


//------------------------------------------------------------------------------
// Represents a hazard pointer list.
template <typename T, typename Alloc>
class hazard_pointer_list
{
public:
    using this_type = hazard_pointer_list;
    using value_type = T;
    using pointer = std::add_pointer_t<value_type>;
    using const_pointer = std::add_pointer_t<std::add_const_t<value_type>>;
    using allocator = Alloc;

public:
    constexpr hazard_pointer_list(allocator& alloc) noexcept;

    // Destructor; delete the hazard pointer list, but NOT the hazard
    // pointers themselves.
    ~hazard_pointer_list();

    // Acquire a new hazard pointer entry.
    hazard_pointer<T>& acquire();

    // Returns true if a particular pointer is contained in the hazard
    // pointer list.
    bool contains(const_pointer const ptr);

private:
    using allocator_traits = typename ::std::allocator_traits<allocator>;
    using hazard_pointer_t = std::add_pointer_t<hazard_pointer<T>>;
    volatile hazard_pointer_t m_head;
    allocator m_allocator;
};

//------------------------------------------------------------------------------
/**
*/
template <typename T, typename Alloc>
constexpr 
hazard_pointer_list<T, Alloc>::hazard_pointer_list(allocator& alloc) noexcept
    : m_head(nullptr)
    , m_allocator(alloc)
{}

//------------------------------------------------------------------------------
/**
*/
template <typename T, typename Alloc>
hazard_pointer_list<T, Alloc>::~hazard_pointer_list()
{
    auto ptr =
        threading::atomic_fetch_store_seq(this->m_head, nullptr);

    hazard_pointer_t next_ptr(nullptr);

    for(; ptr; ptr = next_ptr)
    {
        next_ptr = ptr->next;
        allocator_traits::destroy(this->m_allocator, ptr);
        allocator_traits::deallocate(this->m_allocator, ptr, 1);
    }
}

//------------------------------------------------------------------------------
/**
*/
template <typename T, typename Alloc>
inline hazard_pointer<T>& 
hazard_pointer_list<T, Alloc>::acquire()
{
    auto p = threading::atomic_fetch_acq(&this->m_head);
    auto inactive = false;

    for(; p; p = p->next)
    {
        if(p->is_active.load())
        {
            continue;
        }

        // only try once for each pointer
        if(!p->is_active.compare_exchange_weak(inactive, true))
        {
            continue;
        }

        return *p;
    }

    // need to allocate a new hazard pointer
    p = allocator_traits::allocate(this->m_allocator, 1);
    allocator_traits::construct(this->m_allocator, p);

    hazard_pointer_t head_hp = nullptr;

    // link this hazard pointer in as the m_head of the list.
    do
    {
        head_hp = m_head.load();
        p->next = head_hp;
    } while(!m_head.compare_exchange_weak(head_hp, p));

    return *p;
}

//------------------------------------------------------------------------------
/**
*/
template <typename T, typename Alloc>
inline bool 
hazard_pointer_list<T, Alloc>::contains(const_pointer const ptr)
{
    hazard_pointer_t p = threading::atomic_fetch_acq(this->m_head);
    for(; p; p = p->next)
    {
        bool is_active = threading::atomic_fetch_acq(p->m_is_active);
        if(!is_active)
        {
            continue;
        }

        hazard_pointer_t hp = threading::atomic_fetch_acq(p->m_hazardous_pointer);
        bool are_hp_and_pointer_same = (hp == ptr);

        if(are_hp_and_pointer_same)
        {
            return true;
        }
    }

    return false;
}

} // namespace az::corlib::memory
//------------------------------------------------------------------------------