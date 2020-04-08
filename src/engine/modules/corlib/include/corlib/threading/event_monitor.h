// This file is a part of xray-ng engine
//

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