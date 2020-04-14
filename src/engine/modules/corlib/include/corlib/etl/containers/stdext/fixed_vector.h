// This file is a part of xray-ng engine
//

#pragma once

#include "EASTL/fixed_vector.h"

//-----------------------------------------------------------------------------------------------------------
namespace xr::etl::containers::stdext
{

//-----------------------------------------------------------------------------------------------------------
template<typename Type, size_t Count>
using fixed_vector = eastl::fixed_vector<Type, Count, false, eastl::dummy_allocator>;

} // namespace xr::etl::containers::std
//-----------------------------------------------------------------------------------------------------------
