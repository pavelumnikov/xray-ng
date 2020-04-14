// This file is a part of xray-ng engine
//

#include "corlib/threading/atomic_do_once.h"

//-----------------------------------------------------------------------------------------------------------
namespace xr::threading::internal
{

//-----------------------------------------------------------------------------------------------------------
/**
*/
void run_initializer(etl::action_func f, atomic_do_once_state& state)
{
    // Run function handle
    f();
    // Do atomic exchanging of value
    threading::atomic_store_rel(state, do_once_state::executed);
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
void run_initializer(etl::prediction_func f, atomic_do_once_state& state)
{
    // Run function handle
    auto const result = f();
    // Do atomic exchanging of value
    result ? threading::atomic_store_rel(state, do_once_state::executed) :
        threading::atomic_store_rel(state, do_once_state::uninitialized);
}

} // namespace xr::threading::internal
//-----------------------------------------------------------------------------------------------------------
