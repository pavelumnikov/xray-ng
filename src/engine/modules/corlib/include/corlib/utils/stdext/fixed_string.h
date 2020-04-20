// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/utils/hash_string.h"
#include "corlib/utils/string_view.h"
#include "EASTL/fixed_string.h"

//-----------------------------------------------------------------------------------------------------------
namespace xr::utils::stdext
{

//-----------------------------------------------------------------------------------------------------------
template<int Count>
using fixed_string = eastl::fixed_string<char, Count>;

//-----------------------------------------------------------------------------------------------------------
template<int Count>
using fixed_wstring = eastl::fixed_string<wchar_t, Count>;

//-----------------------------------------------------------------------------------------------------------
using fixed_string32 = fixed_string<32>;
using fixed_wstring32 = fixed_wstring<32>;

//-----------------------------------------------------------------------------------------------------------
using fixed_string64 = fixed_string<64>;
using fixed_wstring64 = fixed_wstring<64>;

//-----------------------------------------------------------------------------------------------------------
using fixed_string128 = fixed_string<128>;
using fixed_wstring128 = fixed_wstring<128>;

//-----------------------------------------------------------------------------------------------------------
using fixed_string256 = fixed_string<256>;
using fixed_wstring256 = fixed_wstring<256>;

//-----------------------------------------------------------------------------------------------------------
using fixed_string512 = fixed_string<512>;
using fixed_wstring512 = fixed_wstring<512>;

//-----------------------------------------------------------------------------------------------------------
/**
*/
template<int Count>
utils::string_view to_string_view(fixed_string<Count> const& s) noexcept
{
    return { eastl::begin(s), eastl::size(s) };
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
template<int Count>
utils::wstring_view to_wstring_view(fixed_wstring<Count> const& s) noexcept
{
    return { eastl::begin(s), eastl::size(s) };
}

} // namespace xr::utils
//-----------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------
namespace eastl
{

//-----------------------------------------------------------------------------------------------------------
template<int N>
struct hash<xr::utils::stdext::fixed_string<N>>
{
    size_t operator()(xr::utils::stdext::fixed_string<N> const& s) const;
}; // struct hash<xr::utils::stdext::fixed_string<N>>

//-----------------------------------------------------------------------------------------------------------
/**
*/
template<int N>
size_t hash<xr::utils::stdext::fixed_string<N>>::operator()(xr::utils::stdext::fixed_string<N> const& s) const
{
    return xr::utils::hash_string_func(s);
}


//-----------------------------------------------------------------------------------------------------------
template<size_t N>
struct hash<xr::utils::stdext::fixed_wstring<N>>
{
    size_t operator()(xr::utils::stdext::fixed_wstring<N> const& s) const;
}; // struct hash<xr::utils::stdext::fixed_wstring<N>>

//-----------------------------------------------------------------------------------------------------------
/**
*/
template<size_t N>
size_t hash<xr::utils::stdext::fixed_wstring<N>>::operator()(xr::utils::stdext::fixed_wstring<N> const& s) const
{
    return xr::utils::hash_string_func(s);
}

} // namespace eastl
//-----------------------------------------------------------------------------------------------------------
