// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/threading/spin_wait.h"
#include "corlib/threading/scoped_lock.h"

//------------------------------------------------------------------------------
namespace xr::threading
{

//------------------------------------------------------------------------------
class multi_threaded_traits final
{
public:
    using locker = spin_wait_fairness;
    using raii = scoped_lock<locker>;
}; // class multi_threaded_traits

//------------------------------------------------------------------------------
class single_threaded_traits final
{
public:
    using locker = spin_wait_noop;
    using raii = scoped_lock<locker>;
}; // class single_threaded_traits

//------------------------------------------------------------------------------
template<typename Locker>
class custom_threaded_traits final
{
public:
    using locker = Locker;
    using raii = scoped_lock<locker>;
}; // class custom_threaded_traits

} // namespace xr::etl::containers
//------------------------------------------------------------------------------
