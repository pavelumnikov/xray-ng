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

#include "corlib/threading/interlocked.h"
#include "corlib/threading/event.h"

//------------------------------------------------------------------------------
namespace xr::threading
{

class event_monitor final
{
public:
    class ticket
    {
        friend class event_monitor;
        volatile size_t aba_epoch;
    };

    event_monitor() noexcept;

    XR_DECLARE_DELETE_COPY_ASSIGNMENT(event_monitor);
    XR_DECLARE_DEFAULT_MOVE_ASSIGNMENT(event_monitor);

    //! Prepare waiting for incoming signal for thread blocking
    void prepare(ticket& ticket) noexcept;
    void commit(ticket& ticket) noexcept;
    void cancel() noexcept;
    void notify() noexcept;

private:
    ticket m_ticket;
    volatile bool m_wait_status;
    bool m_is_spurious;
    event m_event;
};

//------------------------------------------------------------------------------
/**
*/
inline
event_monitor::event_monitor() noexcept
    : m_ticket {}
    , m_wait_status {}
    , m_is_spurious { false }
{
    m_ticket.aba_epoch = { 0 };
    m_wait_status = { false };
}

//------------------------------------------------------------------------------
/**
*/
inline void
event_monitor::prepare(ticket& ticket) noexcept
{
    if(this->m_is_spurious)
    {
        this->m_is_spurious = false;
        (void)this->m_event.wait();
    }

    threading::atomic_store_relax(m_ticket.aba_epoch, ticket.aba_epoch);
    threading::atomic_store_rel(m_wait_status, true);
}

//------------------------------------------------------------------------------
/**
*/
inline void
event_monitor::commit(ticket& ticket) noexcept
{
    auto const current_epoch = threading::atomic_fetch_acq(m_ticket.aba_epoch);
    auto const tickets_epoch = threading::atomic_fetch_acq(ticket.aba_epoch);
    auto const do_it = current_epoch == tickets_epoch;

    if(do_it)
        (void)this->m_event.wait();
    else
        this->cancel();
}

//------------------------------------------------------------------------------
/**
*/
inline void
event_monitor::cancel() noexcept
{
    this->m_is_spurious = threading::atomic_fetch_store_acqrel(m_wait_status, false);
}

//------------------------------------------------------------------------------
/**
*/
inline void
event_monitor::notify() noexcept
{
    threading::atomic_fetch_add_seq<size_t>(m_ticket.aba_epoch, 1U);
    if(threading::atomic_fetch_store_acqrel(m_wait_status, false))
        this->m_event.set(true);
}

} // namespace xr::threading
//------------------------------------------------------------------------------