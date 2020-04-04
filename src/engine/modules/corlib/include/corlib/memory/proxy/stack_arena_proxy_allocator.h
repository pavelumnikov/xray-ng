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

#include "corlib/types.h"

namespace xr::memory::proxy
{

constexpr size_t default_alignment = alignof(double);

template<typename T>
struct select_alignment
{
    static constexpr size_t value = alignof(T) <= default_alignment ? default_alignment : alignof(T);
};

template<typename T>
constexpr auto select_alignment_v = select_alignment<T>::value;

template<typename T, size_t N, size_t alignment = select_alignment_v<T>>
class stack_arena_proxy_allocator
{
    XR_ALIGNAS(alignment) uint8_t buf_[N];
    char* ptr_;

public:
    ~stack_arena_proxy_allocator()
    {
        ptr_ = nullptr;
    }

    stack_arena_proxy_allocator() noexcept : ptr_(buf_)
    {}

    XR_DECLARE_DELETE_COPY_ASSIGNMENT(stack_arena_proxy_allocator);
    XR_DECLARE_DELETE_MOVE_ASSIGNMENT(stack_arena_proxy_allocator);


    template <size_t ReqAlign> uint8_t* allocate(size_t n);
    template <size_t ReqAlign> T* allocate();

    void deallocate(uint8_t* p, size_t n) noexcept;
    void deallocate(T* p) noexcept;

    static constexpr size_t size() noexcept
    {
        return N;
    }

    size_t used() const noexcept
    {
        return static_cast<size_t>(ptr_ - buf_);
    }

    void reset() noexcept
    {
        ptr_ = buf_;
    }

private:
    static size_t align_up(size_t n) noexcept
    {
        return (n + (alignment - 1)) & ~(alignment - 1);
    }

    bool pointer_in_buffer(char* p) noexcept
    {
        return buf_ <= p && p <= buf_ + N;
    }
};

template <typename T, size_t N, size_t alignment>
template <size_t ReqAlign>
inline uint8_t*
stack_arena_proxy_allocator<T, N, alignment>::allocate(size_t n)
{
    static_assert(ReqAlign <= alignment, "alignment is too small for this arena");
    assert(pointer_in_buffer(ptr_) && "short_alloc has outlived arena");
    auto const aligned_n = align_up(n);
    if(static_cast<decltype(aligned_n)>(buf_ + N - ptr_) >= aligned_n)
    {
        char* r = ptr_;
        ptr_ += aligned_n;
        return r;
    }

    XR_DEBUG_ASSERTION_MSG(false, "Out of stack memory");
    return nullptr;
}

template <typename T, size_t N, size_t alignment>
inline void
stack_arena_proxy_allocator<T, N, alignment>::deallocate(uint8_t* p, size_t n) noexcept
{
    XR_DEBUG_ASSERTION_MSG(pointer_in_buffer(ptr_), "short_alloc has outlived arena");
    n = align_up(n);
    if(p + n == ptr_)
        ptr_ = p;
}

} // namespace xr::memory::proxy
