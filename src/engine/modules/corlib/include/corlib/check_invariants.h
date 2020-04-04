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

#include "corlib/etl/type_traits.h"

//------------------------------------------------------------------------------
namespace xr::detail
{

//------------------------------------------------------------------------------
template<typename T>
class check_invariants_raii
{
public:
    template <class T1>
    check_invariants_raii(T1 const* const object);
    ~check_invariants_raii();

    XR_DECLARE_DELETE_COPY_ASSIGNMENT(check_invariants_raii);
    XR_DECLARE_DELETE_MOVE_ASSIGNMENT(check_invariants_raii);

private:
    T const* m_object;
};

//------------------------------------------------------------------------------
/**
 */
template<typename T>
template<typename T1>
check_invariants_raii<T>::check_invariants_raii(T1 const* const object)
    : m_object { object }
{
    XR_DEBUG_ASSERTION_MSG(m_object != nullptr, "Invalid object to check against");
    m_object->check_invariants();
}

 //------------------------------------------------------------------------------
 /**
  */
template<typename T>
check_invariants_raii<T>::~check_invariants_raii()
{
    m_object->check_invariants();
}

} // namespace xr::detail
//------------------------------------------------------------------------------

#ifdef MASTER_GOLD
#   define XR_CHECK_INVARIANTS(Type)
#else
#   define XR_CHECK_INVARIANTS(Type) \
        ::xr::detail::check_invariants_raii<Type> check_invariants_helper$(this)
#endif // MASTER_GOLD