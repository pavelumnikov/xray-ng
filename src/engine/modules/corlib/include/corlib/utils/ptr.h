// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/types.h"
#include "corlib/memory/proxy/eastl_proxy_allocator.h"

#include "EASTL/shared_ptr.h"
#include "EASTL/unique_ptr.h"

#include <cassert>

//-----------------------------------------------------------------------------------------------------------
namespace xr::memory
{
class base_allocator;
} // namespace xr::memory
//-----------------------------------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------------------------------
namespace xr::utils::details
{

//-----------------------------------------------------------------------------------------------------------
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


//-----------------------------------------------------------------------------------------------------------
/**
*/
template<typename Type>
alloc_deleter<Type>::alloc_deleter(memory::proxy::eastl_proxy_allocator a) noexcept
    : m_allocator { eastl::move(a) }
{}

//-----------------------------------------------------------------------------------------------------------
/**
*/
template<typename Type>
void alloc_deleter<Type>::operator()(pointer p) const
{
    assert(p != nullptr);

    p->~Type();
    this->m_allocator.deallocate(p, sizeof(Type));
}

} // namespace xr::utils::details 
//-----------------------------------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------------------------------
namespace xr::utils
{

//-----------------------------------------------------------------------------------------------------------
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

//-----------------------------------------------------------------------------------------------------------
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

//-----------------------------------------------------------------------------------------------------------
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

} // namespace xr::utils
//-----------------------------------------------------------------------------------------------------------
