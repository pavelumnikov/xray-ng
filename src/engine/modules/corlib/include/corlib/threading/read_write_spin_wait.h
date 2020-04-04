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

#include "corlib/threading/atomic_backoff.h"

//------------------------------------------------------------------------------
namespace xr::threading
{

//------------------------------------------------------------------------------
// read_write_spin_wait provides spin-waiting functionality to prevent kernel-level locking.
// This particular locking scheme performs well when lock contention is low, as the while 
// loop overhead is small and locks are acquired very quickly, but degrades as many callers 
// want the lock and most threads are doing a lot of interlocked spinning. There are also no 
// guarantees that a caller will ever acquire the lock
class read_write_spin_wait final
{
public:
    constexpr read_write_spin_wait() noexcept;

    class reader_access final
    {
        read_write_spin_wait& m_spinwaiter;

    public:
        explicit reader_access(read_write_spin_wait& spin) noexcept;
        void lock() const noexcept;
        signalling_bool try_lock() const noexcept;
        void unlock() const noexcept;
    }; // class reader_access

    class writer_access final
    {
        read_write_spin_wait& m_spinwaiter;

    public:
        explicit writer_access(read_write_spin_wait& spin) noexcept;
        void lock() const noexcept;
        signalling_bool try_lock() const noexcept;
        void unlock() const noexcept;
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

//------------------------------------------------------------------------------
/**
*/
constexpr read_write_spin_wait::read_write_spin_wait() noexcept
    : m_u {}
{
    m_u.split_and_rw.data = 0;
}

//------------------------------------------------------------------------------
/**
*/
inline read_write_spin_wait::reader_access::reader_access(read_write_spin_wait& spin) noexcept
    : m_spinwaiter(spin)
{}

//------------------------------------------------------------------------------
/**
*/
inline read_write_spin_wait::writer_access::writer_access(read_write_spin_wait& spin) noexcept
    : m_spinwaiter(spin)
{}

//------------------------------------------------------------------------------
/**
*/
inline void read_write_spin_wait::reader_access::unlock() const noexcept
{
    (void)atomic_fetch_add_seq<uint16_t>(
        this->m_spinwaiter.m_u.split.my_writes.data, 1);
}

} // namespace xr::threading
//------------------------------------------------------------------------------