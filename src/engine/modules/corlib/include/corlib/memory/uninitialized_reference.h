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

#include "corlib/pointer_cast.h"
#include "corlib/memory/memory_functions.h"
#include "EASTL/memory.h"
#include <cassert>

//------------------------------------------------------------------------------
namespace xr::memory::details
{

//------------------------------------------------------------------------------
template <class Class, class ConstructorClass = int>
class uninitialized_reference_impl
{
public:
    using value_type = Class;
    using pointer = eastl::add_pointer_t<Class>;
    using reference = eastl::add_lvalue_reference_t<Class>;
    using const_pointer = eastl::add_pointer_t<eastl::add_const_t<Class>>;
    using const_reference = eastl::add_lvalue_reference_t<eastl::add_const_t<Class>>;

    uninitialized_reference_impl();
    ~uninitialized_reference_impl() = default;

    uninitialized_reference_impl(uninitialized_reference_impl&&) noexcept;
    uninitialized_reference_impl& operator=(uninitialized_reference_impl&&) noexcept;

    uninitialized_reference_impl& operator= (uninitialized_reference_impl const&) = delete;

    pointer operator->();
    reference operator *();
    const_pointer operator->() const;
    const_reference operator *() const;

    pointer c_ptr();
    const_pointer c_ptr() const;

    reference ref();
    const_reference ref() const;

    bool is_constructed() const;

protected:
    pointer construct();
    void destruct();

private:
    XR_ALIGNAS(16) char m_static_memory[sizeof(Class)];
    reference m_variable;
    bool m_is_constructed;

    template <class T>
    struct friend_helper final
    {
        using type = T;
    };
};

//------------------------------------------------------------------------------
/**
*/
template <class Class, class ConstructorClass>
uninitialized_reference_impl<Class, ConstructorClass>::uninitialized_reference_impl()
    : m_static_memory{}
    , m_variable(*reinterpret_cast<Class*>(m_static_memory))
    , m_is_constructed(false)
{
}

//------------------------------------------------------------------------------
/**
*/
template <class Class, class ConstructorClass>
uninitialized_reference_impl<Class, ConstructorClass>::uninitialized_reference_impl(uninitialized_reference_impl&& rhs) noexcept
    : m_variable(static_cast<Class&>(m_static_memory))
    , m_is_constructed(false)
{
    ::eastl::uninitialized_move_n(::eastl::begin(rhs.m_static_memory), 
        sizeof(Class), ::eastl::begin(m_static_memory));
}

//------------------------------------------------------------------------------
/**
*/
template<class Class, class ConstructorClass>
uninitialized_reference_impl<Class, ConstructorClass>&
uninitialized_reference_impl<Class, ConstructorClass>::operator= (uninitialized_reference_impl&& rhs) noexcept
{
    ::eastl::uninitialized_move_n(::eastl::begin(rhs.m_static_memory),
        sizeof(Class), ::eastl::begin(m_static_memory));

    return *this;
}

//------------------------------------------------------------------------------
/**
*/
template <class Class, class ConstructorClass>
typename uninitialized_reference_impl<Class, ConstructorClass>::pointer
uninitialized_reference_impl<Class, ConstructorClass>::operator->()
{
    XR_DEBUG_ASSERTION(m_is_constructed);
    return &m_variable;
}

//------------------------------------------------------------------------------
/**
*/
template <class Class, class ConstructorClass>
typename uninitialized_reference_impl<Class, ConstructorClass>::reference
uninitialized_reference_impl<Class, ConstructorClass>::operator*()
{
    XR_DEBUG_ASSERTION(m_is_constructed);
    return m_variable;
}

//------------------------------------------------------------------------------
/**
*/
template <class Class, class ConstructorClass>
typename uninitialized_reference_impl<Class, ConstructorClass>::const_pointer
uninitialized_reference_impl<Class, ConstructorClass>::operator->() const
{
    XR_DEBUG_ASSERTION(m_is_constructed);
    return &m_variable;
}

//------------------------------------------------------------------------------
/**
*/
template <class Class, class ConstructorClass>
typename uninitialized_reference_impl<Class, ConstructorClass>::const_reference
uninitialized_reference_impl<Class, ConstructorClass>::operator*() const
{
    XR_DEBUG_ASSERTION(m_is_constructed); 
    return m_variable;
}

//------------------------------------------------------------------------------
/**
*/
template <class Class, class ConstructorClass>
typename uninitialized_reference_impl<Class, ConstructorClass>::pointer
uninitialized_reference_impl<Class, ConstructorClass>::c_ptr()
{
    XR_DEBUG_ASSERTION(m_is_constructed);
    return &m_variable;
}

//------------------------------------------------------------------------------
/**
*/
template <class Class, class ConstructorClass>
typename uninitialized_reference_impl<Class, ConstructorClass>::const_pointer
uninitialized_reference_impl<Class, ConstructorClass>::c_ptr() const
{
    XR_DEBUG_ASSERTION(m_is_constructed);
    return &m_variable;
}

//------------------------------------------------------------------------------
/**
*/
template <class Class, class ConstructorClass>
typename uninitialized_reference_impl<Class, ConstructorClass>::reference 
uninitialized_reference_impl<Class, ConstructorClass>::ref()
{
    XR_DEBUG_ASSERTION(m_is_constructed);
    return m_variable;
}

//------------------------------------------------------------------------------
/**
*/
template <class Class, class ConstructorClass>
typename uninitialized_reference_impl<Class, ConstructorClass>::const_reference 
uninitialized_reference_impl<Class, ConstructorClass>::ref() const
{
    XR_DEBUG_ASSERTION(m_is_constructed);
    return m_variable;
}

//------------------------------------------------------------------------------
/**
*/
template <class Class, class ConstructorClass>
bool uninitialized_reference_impl<Class, ConstructorClass>::is_constructed() const
{
    return m_is_constructed;
}

//------------------------------------------------------------------------------
/**
*/
template <class Class, class ConstructorClass>
typename uninitialized_reference_impl<Class, ConstructorClass>::pointer
uninitialized_reference_impl<Class, ConstructorClass>::construct()
{
    XR_DEBUG_ASSERTION(!m_is_constructed);
    m_is_constructed = true;
    return pointer_cast<Class*>(m_static_memory);
}

//------------------------------------------------------------------------------
/**
*/
template <class Class, class ConstructorClass>
void uninitialized_reference_impl<Class, ConstructorClass>::destruct()
{
    XR_DEBUG_ASSERTION(m_is_constructed);
    m_variable.~Class();
    memory::zero(m_static_memory);
    m_is_constructed = false;
}

} // namespace xr::memory::details
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
namespace xr::memory
{

//------------------------------------------------------------------------------
template <class Class, class ConstructorClass = int>
class uninitialized_reference final
    : public details::uninitialized_reference_impl<Class, ConstructorClass>
{};

//------------------------------------------------------------------------------
template <class Class>
class uninitialized_reference<Class, int>final
    : public details::uninitialized_reference_impl<Class, int>
{
    using super = details::uninitialized_reference_impl<Class, int>;

public:
    using value_type = Class;
    using pointer = eastl::add_pointer_t<Class>;

    pointer construct();
    void destruct();
};

//------------------------------------------------------------------------------
/**
*/
template <class Class>
typename uninitialized_reference<Class, int>::pointer 
uninitialized_reference<Class, int>::construct()
{
    return super::construct();
}

//------------------------------------------------------------------------------
/**
*/
template <class Class>
void
uninitialized_reference<Class, int>::destruct()
{
    super::destruct();
}

//------------------------------------------------------------------------------
/**
*/
template<typename Class, typename... Args>
::eastl::enable_if_t<!::eastl::is_trivially_constructible<Class, Args&&...>::value>
construct_reference(uninitialized_reference<Class>& uninitialized_ref, Args&&... args)
{
    static_assert(eastl::is_constructible_v<Class, Args...>,
        "Cannot construct task with provided arguments");

    new(uninitialized_ref.construct()) Class(args...);
}

//------------------------------------------------------------------------------
/**
*/
template<typename Class>
void
destruct_reference(uninitialized_reference<Class>& uninitialized_ref)
{
    uninitialized_ref.destruct();
}


//------------------------------------------------------------------------------
template<typename Class>
struct scoped_construct final
{
    template<typename... Args>
    explicit scoped_construct(uninitialized_reference<Class>& uninitialized_ref, Args&&... args)
        : m_uninitialized_ref(uninitialized_ref)
    {
        construct_reference(this->m_uninitialized_ref, eastl::forward<Args>(args)...);
    }

    scoped_construct(scoped_construct const&) = delete;
    scoped_construct& operator=(scoped_construct const&) = delete;

    scoped_construct(scoped_construct&&) = delete;
    scoped_construct& operator=(scoped_construct&&) = delete;

    ~scoped_construct()
    {
        destruct_reference(this->m_uninitialized_ref);
    }

private:
    uninitialized_reference<Class>& m_uninitialized_ref;
};

} // namespace xr::memory
//------------------------------------------------------------------------------