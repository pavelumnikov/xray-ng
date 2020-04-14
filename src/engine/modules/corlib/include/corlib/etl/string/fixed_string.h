// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/etl/hashing/std/string_hash.h"
#include "EASTL/fixed_string.h"

//-----------------------------------------------------------------------------------------------------------
namespace xr::etl::string
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
eastl::string_view to_string_view(fixed_string<Count> const& s) noexcept
{
    return { eastl::begin(s), eastl::size(s) };
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
template<int Count>
eastl::wstring_view to_wstring_view(fixed_wstring<Count> const& s) noexcept
{
    return { eastl::begin(s), eastl::size(s) };
}

} // namespace xr::etl::string
//-----------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------
namespace eastl
{

//-----------------------------------------------------------------------------------------------------------
template<int N>
struct hash<xr::etl::string::fixed_string<N>>
{
    size_t operator()(xr::etl::string::fixed_string<N> const& s) const;
}; // struct hash<az::strings::fixed_string<N>>

//-----------------------------------------------------------------------------------------------------------
/**
*/
template<int N>
size_t hash<xr::etl::string::fixed_string<N>>::operator()
(xr::etl::string::fixed_string<N> const& s) const
{
    return xr::etl::hashing::std::string_hash(s);
}


//-----------------------------------------------------------------------------------------------------------
template<size_t N>
struct hash<xr::etl::string::fixed_wstring<N>>
{
    size_t operator()(xr::etl::string::fixed_wstring<N> const& s) const;
}; // struct hash<az::strings::fixed_wstring<N>>

//-----------------------------------------------------------------------------------------------------------
/**
*/
template<size_t N>
size_t hash<xr::etl::string::fixed_wstring<N>>::operator()
(xr::etl::string::fixed_wstring<N> const& s) const
{
    return xr::etl::hashing::std::string_hash(s);
}

} // namespace xr::etl::string
//-----------------------------------------------------------------------------------------------------------
