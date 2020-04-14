// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/platform.h"

//-----------------------------------------------------------------------------------------------------------
namespace xr::details
{

//-----------------------------------------------------------------------------------------------------------
class signalling_bool_impl
{
public:
    constexpr signalling_bool_impl(bool const value) noexcept;
    ~signalling_bool_impl();

    constexpr operator bool() const noexcept;
    constexpr bool operator !() const noexcept;

private:
    static constexpr uint8_t unchecked_flag = 128;
    mutable uint8_t m_value;
}; // class signalling_bool_impl

//-----------------------------------------------------------------------------------------------------------
/**
 */
constexpr
signalling_bool_impl::signalling_bool_impl(bool const value) noexcept
    : m_value((value ? uint8_t(1) : uint8_t(0)) | unchecked_flag)
{}

//-----------------------------------------------------------------------------------------------------------
/**
 */
inline signalling_bool_impl::~signalling_bool_impl()
{
    XR_DEBUG_ASSERTION_MSG((m_value & unchecked_flag) == 0,
        "signalling_bool was not checked! check the return value!");
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
constexpr
signalling_bool_impl::operator bool() const noexcept
{
    m_value &= ~unchecked_flag;
    return (m_value != 0);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
constexpr bool
signalling_bool_impl::operator !() const noexcept
{
    return !(bool)(*this);
}

} // namespace xr::details 
//-----------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------
namespace xr
{

#ifdef MASTER_GOLD
using signalling_bool = bool;
#else // #ifdef MASTER_GOLD
using signalling_bool = details::signalling_bool_impl;
#endif // #ifdef MASTER_GOLD

} // namespace xr
//-----------------------------------------------------------------------------------------------------------
