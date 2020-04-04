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

#if !defined(XRAY_PLATFORM_WINDOWS)
#   error "This code is supported by Windows platform!"
#endif // !defined(XRAY_PLATFORM_WINDOWS)

#include "corlib/memory/memory_crt_allocator.h"
#include "memory_utility_for_arena_win32.h"
#include "corlib/threading/atomic_do_once.h"
#include <new.h>
#include <cassert>

//------------------------------------------------------------------------------
namespace xr::memory
{

//------------------------------------------------------------------------------
namespace
{

static volatile threading::do_once_state crt_already_initialized 
{ 
    threading::do_once_state::uninitialized
};

//------------------------------------------------------------------------------
/**
*/
int out_of_memory(size_t size)
{
    XR_UNREFERENCED_PARAMETER(size);
    // ("not enough memory for arena [c runtime allocator][request=%d]", size);
    return (1);
}

//------------------------------------------------------------------------------
/**
 */
void initialize_crt_allocator()
{
    _set_new_mode(1);
    _set_new_handler(&out_of_memory);
}

//------------------------------------------------------------------------------
/**
*/
__declspec(noinline) __declspec(allocator) void* 
malloc_proxy_func(size_t size)
{
    HANDLE const handle = ::GetProcessHeap();
    return HeapAlloc(handle, 0, size);
}

//------------------------------------------------------------------------------
/**
*/
__declspec(noinline) __declspec(allocator) void*
realloc_proxy_func(void* p, size_t size)
{
    HANDLE const handle = ::GetProcessHeap();
    if(p == nullptr)
        return malloc_proxy_func(size);

    return HeapReAlloc(handle, 0, p, size);
}

//------------------------------------------------------------------------------
/**
*/
__declspec(noinline) void
free_proxy_func(void* p)
{
    HANDLE const handle = ::GetProcessHeap();
    HeapFree(handle, 0, p);
}

} // namespace `anonymous`
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
*/
crt_allocator::crt_allocator() 
    : m_malloc_ptr { malloc_proxy_func }
    , m_free_ptr { free_proxy_func }
    , m_realloc_ptr { realloc_proxy_func }
{
    XR_DEBUG_ASSERTION(this->m_malloc_ptr != nullptr);
    XR_DEBUG_ASSERTION(this->m_free_ptr != nullptr);
    XR_DEBUG_ASSERTION(this->m_free_ptr != nullptr);

    threading::atomic_do_once(initialize_crt_allocator, crt_already_initialized);
}

//------------------------------------------------------------------------------
/**
*/
bool crt_allocator::can_allocate_block(size_t const size) const noexcept
{
    XR_UNREFERENCED_PARAMETER(size);
    return true;
}

//------------------------------------------------------------------------------
/**
*/
size_t crt_allocator::total_size() const noexcept
{
    HANDLE const heap = ::GetProcessHeap();
    return (mem_usage(heap, nullptr, nullptr));
}

//------------------------------------------------------------------------------
/**
*/
size_t crt_allocator::allocated_size() const noexcept
{
    HANDLE const heap = ::GetProcessHeap();
    return (mem_usage(heap, nullptr, nullptr));
}

} // namespace xr::memory
//------------------------------------------------------------------------------