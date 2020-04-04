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

#include "EASTL/functional.h"
#include "corlib/platform.h"

namespace xr::etl
{

//
// Delegate interface inspired by works of marcmo (github.com/marcmo/delegates) 
// and Stefan Reinalter (https://blog.molecular-matters.com/)
//
template <typename T>
class pseudo_virtual_pointer final
{};

template <typename R, typename... Params>
class pseudo_virtual_pointer< R(Params...) > final
{
    using instance_pointer = void*;
    typedef R(*internal_function) (void* instance, Params...);
    typedef eastl::pair< void*, internal_function > method_stub;

    template <class C, R(C::*func)(Params...) >
    static R class_method_stub(void* instance, Params... args)
    {
        return (static_cast<C*>(instance)->*func)(args...);
    }

public:
    constexpr pseudo_virtual_pointer() 
        : m_stub { nullptr, nullptr }
    {}

    XR_DECLARE_DEFAULT_DESTRUCTOR(pseudo_virtual_pointer);
    XR_DECLARE_DEFAULT_MOVE_ASSIGNMENT(pseudo_virtual_pointer);
    XR_DECLARE_DELETE_COPY_ASSIGNMENT(pseudo_virtual_pointer);

    template <class C, R(C::*func)(Params...) >
    void bind(C *instance)
    {
        m_stub.first = instance;
        m_stub.second = &class_method_stub< C, func >;
    }

    R invoke(Params... args) const
    {
        return m_stub.second(m_stub.first, args...);
    }

private:
    method_stub m_stub;
};

} // namespace xr::etl
