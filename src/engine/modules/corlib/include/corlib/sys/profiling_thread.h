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

#include "corlib/sys/thread.h"
#include "corlib/math/color_table.h"

//------------------------------------------------------------------------------
namespace xr::sys
{

//------------------------------------------------------------------------------
/**
*/
void register_thread_id(uint32_t thread_id, eastl::string_view desc, math::color col);

//------------------------------------------------------------------------------
/**
*/
void unregister_thread_id(uint32_t thread_id);


//------------------------------------------------------------------------------
struct raii_register_thread final
{
    explicit raii_register_thread(eastl::string_view desc, math::color col) noexcept
        : m_thread_id(sys::current_thread_id())
    {
        register_thread_id(this->m_thread_id, desc, col);
    }
    
    XR_DECLARE_DELETE_COPY_ASSIGNMENT(raii_register_thread);
    XR_DECLARE_DELETE_MOVE_ASSIGNMENT(raii_register_thread);

    ~raii_register_thread()
    {
        unregister_thread_id(this->m_thread_id);
    }

private:
    uint32_t m_thread_id;
};

} // namespace xr::profiling
//------------------------------------------------------------------------------