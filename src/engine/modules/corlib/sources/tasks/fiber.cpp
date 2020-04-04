/*

  Copyright (c) 2020, Pavel Umnikov
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

#include "fiber.h"
#include "corlib/threading/interlocked.h"
#include "../os_include_win32.h"

#if XRAY_PLATFORM_64BIT
#define ReadTeb(offset) __readgsqword(offset);
#define WriteTeb(offset, v) __writegsqword(offset, v)

#else
#define ReadTeb(offset) __readfsdword(offset);
#define WriteTeb(offset, v) __writefsdword(offset, v)

#endif

//------------------------------------------------------------------------------
namespace xr::tasks
{

//------------------------------------------------------------------------------
namespace
{

//------------------------------------------------------------------------------
/**
 */
inline void*
fiber_get_self()
{
    BOOL is_thread_a_fiber = ::IsThreadAFiber();
    if(is_thread_a_fiber == 0)
    {
        // GetCurrentFiber() return invalid values is current thread is not fiber.
        return nullptr;
    }

    //This function is equal to GetCurrentFiber() macro
    auto fiber = (void*)ReadTeb(FIELD_OFFSET(NT_TIB, FiberData));
    return fiber;
}

} // anonymous namespace

//------------------------------------------------------------------------------
/**
 */
void
fiber::switch_to(fiber& from, fiber& to)
{
    XR_MEMORY_FULLCONSISTENCY_BARRIER;

    XR_DEBUG_ASSERTION_MSG(from.m_fiber != INVALID_FIBER, "Invalid from fiber");
    XR_DEBUG_ASSERTION_MSG(to.m_fiber != INVALID_FIBER, "Invalid to fiber");

    ::SwitchToFiber(to.m_fiber);
}

//------------------------------------------------------------------------------
/**
 */
void
fiber::create_from_thread_and_run(fiber_proc_t proc, pvoid arg)
{
    XR_DEBUG_ASSERTION_MSG(!m_valid, "Fiber already created");
    reset(nullptr, nullptr);

    pvoid fiber = fiber_get_self();
    if(fiber == INVALID_FIBER)
    {
        m_fiber = ::ConvertThreadToFiberEx(nullptr, FIBER_FLAG_FLOAT_SWITCH);
        XR_DEBUG_ASSERTION_MSG(m_fiber != INVALID_FIBER, "Can't create fiber");
    }
    else
    {
        m_fiber = fiber;
    }

    m_valid = true;
    proc(arg);
    cleanup();
}

//------------------------------------------------------------------------------
/**
 */
void
fiber::create(size_t stack, size_t reserve, fiber_proc_t proc, pvoid arg)
{
    XR_DEBUG_ASSERTION_MSG(!m_valid, "Fiber already created");

    reset(proc, arg);
    m_fiber = ::CreateFiberEx(stack, reserve, FIBER_FLAG_FLOAT_SWITCH,
        fiber_func_internal, this);

    XR_DEBUG_ASSERTION_MSG(m_fiber != INVALID_FIBER, "Can't create fiber");
    m_valid = true;
}

//------------------------------------------------------------------------------
/**
 */
void
fiber::reset(fiber_proc_t proc, pvoid arg)
{
    m_func = proc;
    m_func_data = arg;
}

//------------------------------------------------------------------------------
/**
 */
void
fiber::cleanup() noexcept
{
    if(m_valid)
    {
        // do not destroy fibers created using ::ConvertThreadToFiberEx
        if(m_func)
        {
            ::DeleteFiber(m_fiber);
        }

        m_fiber = INVALID_FIBER;
        m_valid = false;
    }
}

} // namespace xr::tasks
//------------------------------------------------------------------------------