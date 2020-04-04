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

#pragma once

#include "corlib/threading/interlocked.h"
#include "corlib/etl/type_traits.h"

//------------------------------------------------------------------------------
namespace xr::tasks
{

template<typename T, size_t kBoundedSize>
class mpmc_queue
{
private:
    static_assert(eastl::is_nothrow_copy_assignable<T>::value ||
        eastl::is_nothrow_move_assignable_v<T>,
        "T must be nothrow copy or move assignable");

    static_assert(eastl::is_nothrow_destructible<T>::value,
        "T must be nothrow destructible");

public:
    constexpr mpmc_queue();
    XR_DECLARE_DELETE_COPY_ASSIGNMENT(mpmc_queue);

    signalling_bool enqueue(T&& data);
    signalling_bool dequeue(T& data);

private:
    struct cell
    {
        volatile size_t sequence;
        T data;
    };

    static constexpr size_t mask = (kBoundedSize - 1);
    // Align to avoid false sharing between head and tail
    XR_ALIGNAS(XR_MAX_CACHE_LINE_SIZE) volatile size_t m_enqueue_pos;
    XR_ALIGNAS(XR_MAX_CACHE_LINE_SIZE) volatile size_t m_dequeue_pos;
    XR_ALIGNAS(XR_MAX_CACHE_LINE_SIZE) eastl::array<cell, kBoundedSize> m_values;
}; // class mpmc_queue

//------------------------------------------------------------------------------
/**
 */
template<typename T, size_t kBoundedSize>
constexpr mpmc_queue<T, kBoundedSize>::mpmc_queue()
    : m_enqueue_pos { 0 }
    , m_dequeue_pos { 0 }
    , m_values {}
{
    for(size_t i = 0; i < eastl::size(m_values); ++i)
        m_values[i].sequence = i;
}

//------------------------------------------------------------------------------
/**
 */
template<typename T, size_t kBoundedSize>
inline signalling_bool 
mpmc_queue<T, kBoundedSize>::enqueue(T&& data)
{
    cell* c = nullptr;
    auto pos = threading::atomic_fetch_acq(m_enqueue_pos);
    for(;;)
    {
        c = &m_values[pos & mask];
        auto seq = threading::atomic_fetch_acq(c->sequence);
        intptr_t diff = (seq - pos);
        if(!diff)
        {
            if(threading::atomic_bcas_seq(m_enqueue_pos, pos + 1, pos))
                break;
        }
        else if(diff < 0)
        {
            return false;
        }
        else
        {
            pos = threading::atomic_fetch_acq(m_enqueue_pos);
        }
    }

    c->data = eastl::move(data);
    threading::atomic_store_rel(c->sequence, pos + 1);
    return true;
}

//------------------------------------------------------------------------------
/**
*/
template<typename T, size_t kBoundedSize>
inline signalling_bool
mpmc_queue<T, kBoundedSize>::dequeue(T& data)
{
    cell* c = nullptr;
    auto pos = threading::atomic_fetch_acq(m_dequeue_pos);
    for(;;)
    {
        c = &m_values[pos & mask];
        auto seq = threading::atomic_fetch_acq(c->sequence);
        intptr_t diff = (seq - (pos + 1));
        if(!diff)
        {
            if(threading::atomic_bcas_seq(m_dequeue_pos, pos + 1, pos))
                break;
        }
        else if(diff < 0)
        {
            return false;
        }
        else
        {
            pos = threading::atomic_fetch_acq(m_dequeue_pos);
        }
    }

    XR_DEBUG_ASSERTION(c != nullptr);
    data = eastl::move(c->data);
    threading::atomic_store_rel(c->sequence, pos + mask + 1);

    return true;
}

} // namespace xr::tasks
//------------------------------------------------------------------------------
