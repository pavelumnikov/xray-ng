// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/memory/proxy/eastl_proxy_allocator.h"
#include "EASTL/vector.h"

//-----------------------------------------------------------------------------------------------------------
namespace xr::etl::containers::std
{

//-----------------------------------------------------------------------------------------------------------

using vector_allocator = memory::proxy::eastl_proxy_allocator;

//-----------------------------------------------------------------------------------------------------------

template<typename Type>
using vector = eastl::vector<Type, vector_allocator>;

} // namespace xr::etl::containers::std
//-----------------------------------------------------------------------------------------------------------
