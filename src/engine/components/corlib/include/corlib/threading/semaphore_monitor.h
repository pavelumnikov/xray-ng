/*

  Copyright (c) 2018, Pavel Umnikov
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

#include "corlib/threading/spin_wait.h"
#include "corlib/threading/event.h"
#include "corlib/containers/intrusive_double_linked_list.h"

//------------------------------------------------------------------------------
namespace az::corlib::threading
{

//------------------------------------------------------------------------------
//! Pre-thread descriptor for concurrency monitor
class semaphore_monitor_context
{
    friend class semaphore_monitor;

public:
    semaphore_monitor_context();
    ~semaphore_monitor_context();

    semaphore_monitor_context& operator=(const semaphore_monitor_context&) = delete;

public: // we need these public for intrusive
    semaphore_monitor_context* m_next;
    semaphore_monitor_context* m_prev;

private:
    //! The method for lazy initialization of the thread_context's semaphore.
    //! Inlining of the method is undesirable, due to extra instructions for
    //! exception support added at caller side.
    void Initialize();

private:
    event m_event;
    volatile uint32_t m_epoch;
    volatile bool m_in_wait;
    bool m_spurious_state;
    bool m_abort_state;
    bool m_ready_state;
    uintptr_t m_context;
};

//------------------------------------------------------------------------------
//! User-Space semaphore implementation known as semaphore_monitor Object like in C# or Java
class semaphore_monitor
{
    using semaphore_access_sw = default_atomic_backoff;

public:
    semaphore_monitor();
    ~semaphore_monitor();

    semaphore_monitor& operator=(const semaphore_monitor&) = delete;

    //! Prepare wait by inserting 'thr' into the wailt queue
    void prepare_wait(semaphore_monitor_context& ctx, uintptr_t ctxIdx = 0);

    //! Commit wait if event count has not changed; otherwise, cancel wait.
    /** Returns true if committed, false if canceled. */
    bool commit_wait(semaphore_monitor_context& ctx);

    //! Cancel the wait. Removes the thread from the wait queue if not removed yet.
    void cancel_wait(semaphore_monitor_context& ctx);

    //! wait for a condition to be satisfied with waiting-on semaphore_monitor_context
    template< typename Time, typename Ctx >
    void wait(Time waitUntile, Ctx ctx);

    //! notify one thread about the event
    void notify_one();

    //! notify one thread about the event. Relaxed version.
    void notify_one_relaxed();

    //! notify all waiting threads of the event
    void notify_all();

    //! notify all waiting threads of the event. Relaxed version
    void notify_all_relaxed();

    //! notify waiting threads of the event that satisfies the given predicate
    template< typename Predicate >
    void notify(Predicate& predicate);

    //! notify waiting threads of the event that satisfies the given predicate. Relaxed version
    template< typename Predicate >
    void notify_relaxed(Predicate& predicate);

    //! Abort any sleeping threads at the time of the call
    void abort_all();

    //! Abort any sleeping threads at the time of the call. Relaxed version
    void abort_all_relaxed();

private:
    using semaphore_monitor_context_list = containers::intrusive_double_linked_list< 
        semaphore_monitor_context, 
        semaphore_monitor_context, 
        &semaphore_monitor_context::m_prev, 
        &semaphore_monitor_context::m_next>;

    semaphore_access_sw m_mutex;
    semaphore_monitor_context_list m_wait_set;
    uint32_t m_epoch;
};

//------------------------------------------------------------------------------
/**
*/
inline semaphore_monitor_context::semaphore_monitor_context()
    : m_spurious_state(false)
    , m_abort_state(false)
    , m_ready_state(false)
    , m_context(0)
{
    interlocked::store< memory_order::release >(this->m_epoch, 0);
    interlocked::store< memory_order::release >(this->m_in_wait, false);
}

//------------------------------------------------------------------------------
/**
*/
inline semaphore_monitor_context::~semaphore_monitor_context(void)
{
    if(this->m_ready_state)
    {
        if(this->m_spurious_state)
            this->m_event.wait();
    }
}

//------------------------------------------------------------------------------
/**
*/
template< typename Time, typename Ctx >
inline void 
semaphore_monitor::wait(Time waitUntil, Ctx ctx)
{
    auto slept = false;
    semaphore_monitor_context localCtx;

    this->prepare_wait(localCtx, ctx());

    while(!waitUntil())
    {
        if((slept = this->commit_wait()) != false)
        {
            if(waitUntile())
            {
                break;
            }
        }

        slept = false;
        this->prepare_wait(localCtx, ctx());
    }

    if(!slept)
    {
        this->cancel_wait(localCtx);
    }
}

//------------------------------------------------------------------------------
/**
*/
inline void 
semaphore_monitor::notify_one()
{
    AZ_MEMORY_READWRITE_BARRIER;
    this->notify_one_relaxed();
}

//------------------------------------------------------------------------------
/**
*/
inline void 
semaphore_monitor::notify_all()
{
    AZ_MEMORY_READWRITE_BARRIER;
    this->notify_all_relaxed();
}

//------------------------------------------------------------------------------
/**
*/
template< typename Predicate >
inline void 
semaphore_monitor::notify_relaxed(Predicate& predicate)
{
    if(this->m_wait_set.empty())
        return;

    semaphore_monitor_context_list list;

    {
        scoped_lock lock(this->m_mutex);
        volatile auto last_epoch = interlocked::fetch< memory_order::relaxed >(this->m_epoch);
        interlocked::store< memory_order::relaxed >(this->m_epoch, last_epoch + 1);

        this->m_wait_set.for_each([=](semaphore_monitor_context* ctx) {
            if(predicate(ctx->m_context))
            {
                this->m_wait_set.Remove(*node);
                interlocked::store< memory_order::release >(ctx->m_in_wait, false);
                list.Add(node);
            }
        });
    }

    list.for_each([=](semaphore_monitor_context* ref) {
        ref->
    });

    end = list.End();
    for(CircularListWithSentinel::Node* node = list.Front(); node != end; node = next)
    {
        next = node->next;
        this->to_thread_context(node)->m_event.Signal();
    }

#if defined( DEBUG )
    list.Clear();
#endif
}

//------------------------------------------------------------------------------
/**
*/
template< typename Predicate >
inline void semaphore_monitor::notify(Predicate& predicate)
{
    AZ_MEMORY_READWRITE_BARRIER;
    this->notify_relaxed< Predicate >(predicate);
}

//------------------------------------------------------------------------------
/**
*/
inline void semaphore_monitor::abort_all(void)
{
    AZ_MEMORY_READWRITE_BARRIER;
    this->abort_all_relaxed();
}

} // namespace Foundation