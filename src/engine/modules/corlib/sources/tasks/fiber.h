// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/threading/interlocked.h"

//-----------------------------------------------------------------------------------------------------------
namespace xr::tasks
{

using fiber_handle_t = pvoid;
using fiber_proc_t = void(__stdcall*)(pvoid);
constexpr fiber_handle_t INVALID_FIBER = nullptr;

//-----------------------------------------------------------------------------------------------------------
class fiber
{
public:
    fiber() = default;
    ~fiber();

    static void switch_to(fiber& from, fiber& to);

    void create_from_thread_and_run(fiber_proc_t proc, pvoid arg);
    void create(size_t stack, size_t reserve, fiber_proc_t proc, pvoid arg);
    void reset(fiber_proc_t proc, pvoid arg);

    bool is_valid() const;
    bool is_constructed() const;

private:
    void cleanup() noexcept;
    static void __stdcall fiber_func_internal(void* arg);

    pvoid m_func_data { nullptr };
    fiber_proc_t m_func { nullptr };
    fiber_handle_t m_fiber { INVALID_FIBER };
    bool m_valid { false };
};

//-----------------------------------------------------------------------------------------------------------
/**
 */
inline fiber::~fiber()
{
    cleanup();
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
inline bool
fiber::is_valid() const
{
    return m_valid;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
inline bool
fiber::is_constructed() const
{
    return m_valid && (m_fiber != INVALID_FIBER);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
inline void __stdcall 
fiber::fiber_func_internal(void* arg)
{
    auto self = reinterpret_cast<fiber*>(arg);

    XR_DEBUG_ASSERTION(self != nullptr);
    XR_DEBUG_ASSERTION(self->m_func != nullptr);
    XR_DEBUG_ASSERTION(self->m_func_data != nullptr);

    self->m_func(self->m_func_data);
}

} // namespace xr::tasks
//-----------------------------------------------------------------------------------------------------------
