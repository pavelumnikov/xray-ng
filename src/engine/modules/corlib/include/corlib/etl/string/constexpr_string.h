// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/etl/string/basic_constexpr_string.h"
#include "corlib/etl/hashing/elsa_hash.h"
#include "EASTL/string_view.h"

//-----------------------------------------------------------------------------------------------------------
namespace xr::etl::string
{

//-----------------------------------------------------------------------------------------------------------
template<size_t N>
using constexpr_string = basic_constexpr_string<char, N>;

//-----------------------------------------------------------------------------------------------------------
template<size_t N>
using constexpr_wstring = basic_constexpr_string<wchar_t, N>;

//-----------------------------------------------------------------------------------------------------------

using constexpr_string32 = constexpr_string<32>;
using constexpr_wstring32 = constexpr_string<32>;

//-----------------------------------------------------------------------------------------------------------

using constexpr_string64 = constexpr_string<64>;
using constexpr_wstring64 = constexpr_wstring<64>;

//-----------------------------------------------------------------------------------------------------------

using constexpr_string128 = constexpr_string<128>;
using constexpr_wstring128 = constexpr_wstring<128>;

//-----------------------------------------------------------------------------------------------------------

using constexpr_string256 = constexpr_string<256>;
using constexpr_wstring256 = constexpr_wstring<256>;

//-----------------------------------------------------------------------------------------------------------

using constexpr_string512 = constexpr_string<512>;
using constexpr_wstring512 = constexpr_wstring<512>;

//-----------------------------------------------------------------------------------------------------------
/**
*/
template<size_t N>
eastl::string_view to_string_view(constexpr_string<N> const& s) noexcept
{
    return { eastl::begin(s), eastl::size(s) };
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
template<size_t N>
eastl::wstring_view to_wstring_view(constexpr_wstring<N> const& s) noexcept
{
    return { eastl::begin(s), eastl::size(s) };
}

} // namespace xr::etl::string
//-----------------------------------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------------------------------
namespace eastl
{

//-----------------------------------------------------------------------------------------------------------
template<size_t N> 
struct hash<xr::etl::string::constexpr_string<N>> final
{
    size_t operator()(xr::etl::string::constexpr_string<N>& s) const;
};

//-----------------------------------------------------------------------------------------------------------
/**
*/
template<size_t N>
size_t hash<xr::etl::string::constexpr_string<N>>::operator()
    (xr::etl::string::constexpr_string<N>& s) const
{
    return xr::etl::hasing::elsa<
        az::strings::constexpr_string<N>>{}(s);
}


//-----------------------------------------------------------------------------------------------------------
template<size_t N>
struct hash<xr::etl::string::constexpr_wstring<N>> final
{
    size_t operator()(xr::etl::string::constexpr_wstring<N>& s) const;
};

//-----------------------------------------------------------------------------------------------------------
/**
*/
template<size_t N>
size_t 
hash<xr::etl::string::constexpr_wstring<N>>::operator()
    (xr::etl::string::constexpr_wstring<N>& s) const
{
    return xr::etl::hashing::elsa<
        xr::etl::string::constexpr_wstring<N>>{}(s);
}

} // namespace std
//-----------------------------------------------------------------------------------------------------------
