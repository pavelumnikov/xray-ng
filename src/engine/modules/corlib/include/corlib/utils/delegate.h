// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/types.h"
#include "EASTL/utility.h"

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, utils)

// Delegate interface inspired by works of marcmo (github.com/marcmo/delegates) 
// and Stefan Reinalter (https://blog.molecular-matters.com/)
//
template <typename T>
class delegate_def {};

//-----------------------------------------------------------------------------------------------------------
template <typename R, typename... Params>
class delegate_def<R(Params...)>
{
public:
    XR_CONSTEXPR_CPP14_OR_INLINE delegate_def() XR_NOEXCEPT;

    XR_DECLARE_DEFAULT_DESTRUCTOR(delegate_def);
    XR_DECLARE_DEFAULT_MOVE_ASSIGNMENT(delegate_def);
    XR_DECLARE_DELETE_COPY_ASSIGNMENT(delegate_def);

    template <class C, R(C:: * func)(Params...) >
    void bind(C* instance);

    template <R(* func)(Params...) >
    void bind();

    R invoke(Params... args) const;

private:
    using instance_pointer = pvoid;
    typedef R(*internal_function) (pvoid instance, Params...);
    typedef eastl::pair<pvoid, internal_function> method_stub;

    template<typename C, R(C:: * func)(Params...) >
    static R class_method_stub(pvoid instance, Params... args);

    template<R(*func)(Params...) >
    static R function_stub(pvoid instance, Params... args);

    method_stub m_stub;
}; // class delegate_def<R(Args...)>

//-----------------------------------------------------------------------------------------------------------
// Declare delegate as a class template. It will be specialized
// later for all number of arguments like for boost::function.
template <typename R, typename ... Args>
using delegate = delegate_def<R(Args...)>;

//-----------------------------------------------------------------------------------------------------------
/**
 */
template <typename R, typename... Params>
constexpr delegate_def<R(Params...)>::delegate_def()
    : m_stub(nullptr, nullptr)
{}

//-----------------------------------------------------------------------------------------------------------
/**
 */
template <typename R, typename... Params>
template <class C, R(C:: * func)(Params...) >
inline void delegate_def<R(Params...)>::bind(C* instance)
{
    m_stub.first = instance;
    m_stub.second = &class_method_stub< C, func >;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
template <typename R, typename... Params>
template <R(*func)(Params...) >
inline void delegate_def<R(Params...)>::bind()
{
    m_stub.second = &function_stub< func >;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
template <typename R, typename... Params>
inline R delegate_def<R(Params...)>::invoke(Params... args) const
{
    return m_stub.second(m_stub.first, args...);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
template <typename R, typename... Params>
template<typename C, R(C:: * func)(Params...) >
R delegate_def<R(Params...)>::class_method_stub(pvoid instance, Params... args)
{
    return (static_cast<C*>(instance)->*func)(args...);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
template <typename R, typename... Params>
template<R(*func)(Params...) >
R delegate_def<R(Params...)>::function_stub(pvoid instance, Params... args)
{
    return (*func)(args...);
}

XR_NAMESPACE_END(xr, utils)
//-----------------------------------------------------------------------------------------------------------
