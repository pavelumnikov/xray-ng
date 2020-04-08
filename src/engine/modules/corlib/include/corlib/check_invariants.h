// This file is a part of xray-ng engine
//

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