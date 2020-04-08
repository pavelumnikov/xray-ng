// This file is a part of xray-ng engine
//

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
