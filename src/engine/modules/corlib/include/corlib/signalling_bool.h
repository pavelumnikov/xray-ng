/*

  Copyright (c) 2019, Pavel Umnikov
  All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:

  * Redistributions of source code must retain the above copyright notice,
    this list of conditions and the following disclaimer.
  * Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.

  THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND ANY
  EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  DISCLAIMED. IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE FOR ANY
  DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
  OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
  DAMAGE.

*/

#pragma once

#include "corlib/platform.h"

//------------------------------------------------------------------------------
namespace xr::details
{

//------------------------------------------------------------------------------
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

//------------------------------------------------------------------------------
/**
 */
constexpr
signalling_bool_impl::signalling_bool_impl(bool const value) noexcept
    : m_value((value ? uint8_t(1) : uint8_t(0)) | unchecked_flag)
{}

//------------------------------------------------------------------------------
/**
 */
inline signalling_bool_impl::~signalling_bool_impl()
{
    XR_DEBUG_ASSERTION_MSG((m_value & unchecked_flag) == 0,
        "signalling_bool was not checked! check the return value!");
}

//------------------------------------------------------------------------------
/**
 */
constexpr
signalling_bool_impl::operator bool() const noexcept
{
    m_value &= ~unchecked_flag;
    return (m_value != 0);
}

//------------------------------------------------------------------------------
/**
 */
constexpr bool
signalling_bool_impl::operator !() const noexcept
{
    return !(bool)(*this);
}

} // namespace xr::details 
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
namespace xr
{

#ifdef MASTER_GOLD
using signalling_bool = bool;
#else // #ifdef MASTER_GOLD
using signalling_bool = details::signalling_bool_impl;
#endif // #ifdef MASTER_GOLD

} // namespace xr
//------------------------------------------------------------------------------