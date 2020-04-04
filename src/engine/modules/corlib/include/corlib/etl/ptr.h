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
#include "corlib/memory/proxy/eastl_proxy_allocator.h"

#include "EASTL/shared_ptr.h"
#include "EASTL/unique_ptr.h"

#include <cassert>

//------------------------------------------------------------------------------
namespace xr::memory
{
class base_allocator;
} // namespace xr::memory
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
namespace xr::etl::details
{

//------------------------------------------------------------------------------
template<typename Type>
class alloc_deleter final
{
public:
    using pointer = eastl::add_pointer_t<Type>;

    explicit alloc_deleter(memory::proxy::eastl_proxy_allocator a) noexcept;
    void operator()(pointer p) const;

private:
    memory::proxy::eastl_proxy_allocator m_allocator;
}; // struct alloc_deleter


//------------------------------------------------------------------------------
/**
*/
template<typename Type>
alloc_deleter<Type>::alloc_deleter(memory::proxy::eastl_proxy_allocator a) noexcept
    : m_allocator { eastl::move(a) }
{}

//------------------------------------------------------------------------------
/**
*/
template<typename Type>
void alloc_deleter<Type>::operator()(pointer p) const
{
    assert(p != nullptr);

    p->~Type();
    this->m_allocator.deallocate(p, sizeof(Type));
}

} // namespace xr::etl::details 
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
namespace xr::etl
{

//------------------------------------------------------------------------------
// owner
//
// owner<T> is designed as a bridge for code that must deal directly with owning
// pointers for some reason
//
// T must be a pointer type
// - disallow construction from any type other than pointer type
//
template <class T, class = eastl::enable_if_t<eastl::is_pointer<T>::value>>
using owner = T;

template<typename T>
using unique_ptr = eastl::unique_ptr<T, details::alloc_deleter<T>>;

template<typename T>
using shared_ptr = eastl::shared_ptr<T>;

template<typename T>
using weak_ptr = eastl::weak_ptr<T>;

//------------------------------------------------------------------------------
/**
*/
template<typename RetType, typename... Args>
auto make_shared_ptr(memory::base_allocator& alloc, Args&& ... args)
{
    using return_type = RetType;

    memory::proxy::eastl_proxy_allocator proxy_allocator(alloc);
    return eastl::allocate_shared<return_type>(eastl::move(proxy_allocator),
        eastl::forward<Args&&>(args)...);
}

//------------------------------------------------------------------------------
/**
*/
template<typename RetType, typename... Args>
auto make_unique_ptr(memory::base_allocator& alloc, Args&& ... args)
{
    using return_type = RetType;
    using unique = unique_ptr<return_type>;

    memory::proxy::eastl_proxy_allocator proxy_allocator(alloc);
    auto* p = new(proxy_allocator.allocate(sizeof(return_type))) return_type(args...);

    typename unique::deleter_type deleter(eastl::move(proxy_allocator));
    return unique(p, eastl::move(deleter));
}


// This is a helper class which should be used in case a class derives from a 
// base class which derives from enable_shared_from_this
// If Derived will also inherit enable_shared_from_this, it will cause multiple 
// inheritance from enable_shared_from_this, which results in a runtime errors 
// because we have 2 copies of the WeakPtr inside shared_ptr
template<typename Base, typename Derived>
class inherit_shared_from_this
{
public:
    shared_ptr<Derived> shared_from_this()
    {
        Base* base_ptr = static_cast<Derived*>(this);
        shared_ptr<Base> shared = base_ptr->shared_from_this();
        return eastl::static_pointer_cast<Derived>(shared);
    }

    shared_ptr<const Derived> shared_from_this() const
    {
        Base const* base_ptr = static_cast<const Derived*>(this);
        shared_ptr<const Base> shared = base_ptr->shared_from_this();
        return eastl::static_pointer_cast<const Derived>(shared);
    }
};

} // namespace xr::etl
//------------------------------------------------------------------------------