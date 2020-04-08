// This file is a part of xray-ng engine
//

#pragma once

#include "EASTL/fixed_string.h"

//------------------------------------------------------------------------------
namespace xr::etl::hashing::std
{

//------------------------------------------------------------------------------
/**
*/
template<typename TString>
size_t string_hash(TString const& s)
{
    auto constexpr init =
        size_t(sizeof(size_t) == 8 ? 0xcbf29ce484222325 : 0x811c9dc5);

    auto constexpr multiplier =
        size_t(sizeof(size_t) == 8 ? 0x100000001b3 : 0x1000193);

    auto hash = init;
    auto const end = eastl::end(s);

    for(auto current = eastl::begin(s); current != end; ++current)
    {
        hash ^= *current;
        hash *= multiplier;
    }

    return hash;
}

} // namespace xr::etl::hashing::std
//------------------------------------------------------------------------------