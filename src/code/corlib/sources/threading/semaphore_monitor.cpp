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

#include "corlib/threading/semaphore_monitor.h"
#include "corlib/threading/scoped_lock.h"

namespace az::corlib::threading
{

//------------------------------------------------------------------------------
/**
*/
void semaphore_monitor_context::Initialize()
{
    this->m_ready_state = true;
}

//------------------------------------------------------------------------------
/**
*/
semaphore_monitor::semaphore_monitor()
{
    interlocked::store< memory_order::relaxed, unsigned int, unsigned int >(this->m_epoch, 0);
}

//------------------------------------------------------------------------------
/**
*/
semaphore_monitor::~semaphore_monitor()
{
    this->abort_all();
    assert(this->m_wait_set.empty() && "m_wait_set is not empty! Critical invariant!");
}

//------------------------------------------------------------------------------
/**
*/
void semaphore_monitor::prepare_wait(semaphore_monitor_context& ctx, uintptr_t context_id)
{
    if(!ctx.m_ready_state)
    {
        ctx.Initialize();
    }
    else if(ctx.m_spurious_state)
    {
        ctx.m_spurious_state = false;
        ctx.m_event.wait();
    }

    ctx.m_context = context_id;
    interlocked::store< memory_order::release >(ctx.m_in_wait, true);

    //! It's vital to lock mutex here, even m_epoch getting and setting is relaxed. It's still relaxed in 98% of
    //! all events, but 2% still happens in race conditions... Another trick known as memory barrier has been used,
    //! this reduced to 1%, but this is not good at all. 
    {
        scoped_lock lock(this->m_mutex);
        volatile auto last_epoch = interlocked::fetch< memory_order::relaxed >(this->m_epoch);
        interlocked::store< memory_order::relaxed >(ctx.m_epoch, last_epoch);
        this->m_wait_set.push_back(&ctx);
    }
    AZ_MEMORY_READWRITE_BARRIER;
}

//------------------------------------------------------------------------------
/**
*/
bool semaphore_monitor::commit_wait(semaphore_monitor_context& ctx)
{
    unsigned int thisEpoch = interlocked::fetch< memory_order::relaxed >(this->m_epoch);
    unsigned int contextEpoch = interlocked::fetch< memory_order::relaxed >(ctx.m_epoch);
    const bool localProcessState = contextEpoch == thisEpoch;

    // this check is just an optimization
    if(localProcessState)
    {
        assert(ctx.m_ready_state && "Use of commit_wait() without prior prepare_wait()");
        ctx.m_event.wait();

#if defined( _DEBUG )
        auto const queue_assert = interlocked::fetch< memory_order::acquire >(ctx.m_in_wait);
        assert((!queue_assert) && "Still in the queue?");
#endif
        if(ctx.m_abort_state)
        {
            /// TODO: just raise exception and exit from function
            return false;
        }
    }
    else
    {
        this->cancel_wait(ctx);
    }

    return localProcessState;
}

//------------------------------------------------------------------------------
/**
*/
void semaphore_monitor::cancel_wait(semaphore_monitor_context& ctx)
{
    //! Spurious wakeup will be pumped in the following prepare_wait()
    ctx.m_spurious_state = true;

    //! Try to remove node from wait set
    auto const thread_in_wait_set = interlocked::fetch< memory_order::acquire >(ctx.m_in_wait);
    if(thread_in_wait_set)
    {
        scoped_lock< semaphore_access_sw > lock(this->m_mutex);
        //! Successfully removed from waitset, so there will be no spurious wakeup
        interlocked::store< memory_order::release >(ctx.m_in_wait, false);
        ctx.m_spurious_state = false;
        this->m_wait_set.erase(&ctx);        
    }
}

//------------------------------------------------------------------------------
/**
*/
void semaphore_monitor::notify_one_relaxed()
{
    if(this->m_wait_set.empty())
    {
        return;
    }

    CircularListWithSentinel::Node* node;
    const CircularListWithSentinel::Node* end = this->m_wait_set.End();

    {
        scoped_lock< semaphore_access_sw > lock(this->m_mutex);
        volatile auto last_epoch = interlocked::fetch< memory_order::relaxed >(this->m_epoch);
        interlocked::store< memory_order::relaxed >(this->m_epoch, last_epoch + 1);

        node = this->m_wait_set.Front();
        if(node != end)
        {
            this->m_wait_set.Remove(*node);
            interlocked::store< memory_order::release >(this->to_thread_context(node)->m_in_wait, false);
        }
    }

    if(node != end)
    {
        this->to_thread_context(node)->m_event.set(true);
    }
}

//------------------------------------------------------------------------------
/**
*/
void semaphore_monitor::notify_all_relaxed()
{
    if(this->m_wait_set.IsEmpty())
    {
        return;
    }

    CircularListWithSentinel list;
    const CircularListWithSentinel::Node* end;

    {
        scoped_lock< semaphore_access_sw > lock(this->m_mutex);
        volatile unsigned int last_epoch = interlocked::fetch< memory_order::relaxed >(this->m_epoch);
        interlocked::store< memory_order::relaxed >(this->m_epoch, last_epoch + 1);
        this->m_wait_set.MoveTo(list);
        end = list.End();

        for(CircularListWithSentinel::Node* node = list.Front(); node != end; node = node->next)
        {
            interlocked::store< memory_order::release >(this->to_thread_context(node)->m_in_wait, false);
        }
    }

    CircularListWithSentinel::Node* next;
    for(CircularListWithSentinel::Node* node = list.Front(); node != end; node = next)
    {
        next = node->next;
        this->to_thread_context(node)->m_event.set(true);
    }

#if defined( DEBUG )
    list.Clear();
#endif
}

//------------------------------------------------------------------------------
/**
*/
void semaphore_monitor::abort_all_relaxed()
{
    if(this->m_wait_set.IsEmpty())
    {
        return;
    }

    CircularListWithSentinel list;
    const CircularListWithSentinel::Node* end;

    {
        scoped_lock< semaphore_access_sw > lock(this->m_mutex);
        volatile auto last_epoch = interlocked::fetch< memory_order::relaxed >(this->m_epoch);
        interlocked::store< memory_order::relaxed >(this->m_epoch, last_epoch + 1);

        this->m_wait_set.MoveTo(list);
        end = list.End();

        for(CircularListWithSentinel::Node* node = list.Front(); node != end; node = node->next)
        {
            interlocked::store< memory_order::release >(this->to_thread_context(node)->m_in_wait, false);
        }
    }

    CircularListWithSentinel::Node* next;
    for(CircularListWithSentinel::Node* node = list.Front(); node != end; node = next)
    {
        next = node->next;
        this->to_thread_context(node)->m_abort_state = true;
        this->to_thread_context(node)->m_event.set(true);
    }

#if defined( DEBUG )
    list.Clear();
#endif
}

} // namespace az::corlib::threading
//------------------------------------------------------------------------------