// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/utils/details/basic_constexpr_string.h"
#include "corlib/utils/hash_elsa.h"
#include "corlib/utils/string_view.h"

//-----------------------------------------------------------------------------------------------------------
namespace xr::utils
{

//-----------------------------------------------------------------------------------------------------------
template<size_t N>
using constexpr_string = details::basic_constexpr_string<char, N>;

//-----------------------------------------------------------------------------------------------------------
template<size_t N>
using constexpr_wstring = details::basic_constexpr_string<wchar_t, N>;

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
utils::string_view to_string_view(constexpr_string<N> const& s) noexcept
{
    return { eastl::begin(s), eastl::size(s) };
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
template<size_t N>
utils::wstring_view to_wstring_view(constexpr_wstring<N> const& s) noexcept
{
    return { eastl::begin(s), eastl::size(s) };
}

} // namespace xr::utils
//-----------------------------------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------------------------------
namespace eastl
{

//-----------------------------------------------------------------------------------------------------------
template<size_t N> 
struct hash<xr::utils::constexpr_string<N>> final
{
    size_t operator()(xr::utils::constexpr_string<N>& s) const;
};

//-----------------------------------------------------------------------------------------------------------
/**
*/
template<size_t N>
size_t hash<xr::utils::constexpr_string<N>>::operator()(xr::utils::constexpr_string<N>& s) const
{
    return xr::utils::hasing::elsa<xr::utils::constexpr_string<N>>{}(s);
}


//-----------------------------------------------------------------------------------------------------------
template<size_t N>
struct hash<xr::utils::constexpr_wstring<N>> final
{
    size_t operator()(xr::utils::constexpr_wstring<N>& s) const;
};

//-----------------------------------------------------------------------------------------------------------
/**
*/
template<size_t N>
size_t 
hash<xr::utils::constexpr_wstring<N>>::operator()(xr::utils::constexpr_wstring<N>& s) const
{
    return xr::utils::hashing::elsa<xr::utils::constexpr_wstring<N>>{}(s);
}

} // namespace std
//-----------------------------------------------------------------------------------------------------------
