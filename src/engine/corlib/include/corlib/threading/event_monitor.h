// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/threading/interlocked.h"
#include "corlib/threading/event.h"

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, threading)

//-----------------------------------------------------------------------------------------------------------
class event_monitor final
{
public:
    class ticket
    {
        friend class event_monitor;
        atomic_size_t aba_epoch;
    };

    event_monitor() XR_NOEXCEPT;
    XR_DECLARE_DELETE_COPY_ASSIGNMENT(event_monitor);
    XR_DECLARE_DEFAULT_MOVE_ASSIGNMENT(event_monitor);

    //! Prepare waiting for incoming signal for thread blocking
    void prepare(ticket& ticket) XR_NOEXCEPT;
    void commit(ticket& ticket) XR_NOEXCEPT;
    void cancel() XR_NOEXCEPT;
    void notify() XR_NOEXCEPT;

private:
    ticket m_ticket;
    atomic_bool m_wait_status;
    bool m_is_spurious;
    event m_event;
}; // class event_monitor

//-----------------------------------------------------------------------------------------------------------
/**
*/
inline
event_monitor::event_monitor() XR_NOEXCEPT
    : m_ticket {}
    , m_wait_status {}
    , m_is_spurious { false }
{
    m_ticket.aba_epoch = { 0 };
    m_wait_status = { false };
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
inline void
event_monitor::prepare(ticket& ticket) XR_NOEXCEPT
{
    if(m_is_spurious)
    {
        m_is_spurious = false;
        (void)m_event.wait();
    }

    threading::atomic_store_relax(m_ticket.aba_epoch, ticket.aba_epoch);
    threading::atomic_store_rel(m_wait_status, true);
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
inline void
event_monitor::commit(ticket& ticket) XR_NOEXCEPT
{
    auto const current_epoch = threading::atomic_fetch_acq(m_ticket.aba_epoch);
    auto const tickets_epoch = threading::atomic_fetch_acq(ticket.aba_epoch);
    auto const do_it = current_epoch == tickets_epoch;

    if(do_it)
        (void)m_event.wait();
    else
        cancel();
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
inline void
event_monitor::cancel() XR_NOEXCEPT
{
    m_is_spurious = threading::atomic_fetch_store_acqrel(m_wait_status, false);
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
inline void
event_monitor::notify() XR_NOEXCEPT
{
    threading::atomic_fetch_add_seq<size_t>(m_ticket.aba_epoch, 1U);
    if(threading::atomic_fetch_store_acqrel(m_wait_status, false))
        m_event.set(true);
}

XR_NAMESPACE_END(xr, threading)
//-----------------------------------------------------------------------------------------------------------
