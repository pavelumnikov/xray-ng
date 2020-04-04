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

#include "EASTL/string_view.h"
#include "EASTL/optional.h"

//------------------------------------------------------------------------------
namespace xr::etl::string
{

//------------------------------------------------------------------------------
/** Attempts to return internal string as an Int, default if fail
*/
eastl::optional<int32_t> as_int(eastl::string_view s) noexcept;

//------------------------------------------------------------------------------
/** Attempts to return internal string as an Int, default if fail
*/
eastl::optional<int64_t> as_int64(eastl::string_view s) noexcept;

//------------------------------------------------------------------------------
/** Attempts to return internal string as an uint, default if fail
*/
eastl::optional<uint32_t> as_uint(eastl::string_view s) noexcept;

//------------------------------------------------------------------------------
/** Attempts to return internal string as an uint64_t, default if fail
*/
eastl::optional<uint64_t> as_uint64(eastl::string_view s) noexcept;

//------------------------------------------------------------------------------
/** Attempts to return internal string as a float, default if fail
*/
eastl::optional<float> as_float(eastl::string_view s) noexcept;

//------------------------------------------------------------------------------
/** Attempts to return internal string as a double, default if fail
*/
eastl::optional<double> as_double(eastl::string_view s) noexcept;

} // namespace xr::etl::string
//------------------------------------------------------------------------------