// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/threading/atomic_backoff.h"

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, threading)

//-----------------------------------------------------------------------------------------------------------
// read_write_spin_wait provides spin-waiting functionality to prevent kernel-level locking.
// This particular locking scheme performs well when lock contention is low, as the while 
// loop overhead is small and locks are acquired very quickly, but degrades as many callers 
// want the lock and most threads are doing a lot of interlocked spinning. There are also no 
// guarantees that a caller will ever acquire the lock
class read_write_spin_wait final
{
public:
    XR_CONSTEXPR_CPP14_OR_INLINE read_write_spin_wait() XR_NOEXCEPT;

    class reader_access final
    {
        read_write_spin_wait& m_spinwaiter;

    public:
        explicit reader_access(read_write_spin_wait& spin) XR_NOEXCEPT;
        void lock() const XR_NOEXCEPT;
        signalling_bool try_lock() const XR_NOEXCEPT;
        void unlock() const XR_NOEXCEPT;
    }; // class reader_access

    class writer_access final
    {
        read_write_spin_wait& m_spinwaiter;

    public:
        explicit writer_access(read_write_spin_wait& spin) XR_NOEXCEPT;
        void lock() const XR_NOEXCEPT;
        signalling_bool try_lock() const XR_NOEXCEPT;
        void unlock() const XR_NOEXCEPT;
    }; // class writer_access

private:
    friend class reader_access;
    friend class writer_access;

    union
    {
        struct
        {
            interlocked_align_t< uint16_t > my_reads;
            interlocked_align_t< uint16_t > my_writes;
            interlocked_align_t< uint16_t > my_tickets;
        } split;

        interlocked_align_t< uint32_t > rw;
        interlocked_align_t< uint64_t > split_and_rw;
    } m_u;

    static const uint64_t mask = 0xFFFF;
}; // class read_write_spin_wait

//-----------------------------------------------------------------------------------------------------------
/**
*/
XR_CONSTEXPR_CPP14_OR_INLINE read_write_spin_wait::read_write_spin_wait() XR_NOEXCEPT
    : m_u()
{
    m_u.split_and_rw.data = 0;
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
inline read_write_spin_wait::reader_access::reader_access(read_write_spin_wait& spin) XR_NOEXCEPT
    : m_spinwaiter(spin)
{}

//-----------------------------------------------------------------------------------------------------------
/**
*/
inline read_write_spin_wait::writer_access::writer_access(read_write_spin_wait& spin) XR_NOEXCEPT
    : m_spinwaiter(spin)
{}

//-----------------------------------------------------------------------------------------------------------
/**
*/
inline void read_write_spin_wait::reader_access::unlock() const XR_NOEXCEPT
{
    (void)atomic_fetch_add_seq<uint16_t>(m_spinwaiter.m_u.split.my_writes.data, 1);
}

XR_NAMESPACE_END(xr, threading)
//-----------------------------------------------------------------------------------------------------------
