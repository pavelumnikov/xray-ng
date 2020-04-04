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

#include "corlib/threading/read_write_spin_wait.h"

//------------------------------------------------------------------------------
namespace xr::threading
{

//------------------------------------------------------------------------------
/**
*/
void read_write_spin_wait::writer_access::lock() const noexcept
{
    auto const ticket_id = 
        atomic_fetch_add_seq<uint16_t>(m_spinwaiter.m_u.split.my_tickets.data, 1);

    while(ticket_id != atomic_fetch_acq(m_spinwaiter.m_u.split.my_writes.data));
}

//------------------------------------------------------------------------------
/**
*/
signalling_bool
read_write_spin_wait::writer_access::try_lock() const noexcept
{
    uint64_t value;
    auto const old = atomic_fetch_acq(m_spinwaiter.m_u.split_and_rw.data);

    do
    {
        auto const writes = static_cast<uint16_t>(old >> 16);
        auto tickets = static_cast<uint16_t>(old >> 32);

        if(writes != tickets)
            return false;

        tickets++;
        value = (old & ~(mask << 32)) | (static_cast<uint64_t>(tickets) << 32);
    } while(!atomic_bcas_seq<uint64_t>(m_spinwaiter.m_u.split_and_rw.data, value, old));

    return true;
}

//------------------------------------------------------------------------------
/**
*/
void read_write_spin_wait::writer_access::unlock() const noexcept
{
    // This function is implemented this way to avoid an atomic increment
    // and go for a simple load store operation instead.
    auto const reads =
        atomic_fetch_acq(m_spinwaiter.m_u.split.my_reads.data) + 1;

    auto const writes =
        atomic_fetch_acq(m_spinwaiter.m_u.split.my_writes.data) + 1;

    // Note that tickets can still be modified so we can't update d.all.
    auto const rw = static_cast<uint32_t>(reads) | static_cast<uint32_t>(writes) << 16;
    atomic_store_rel<uint32_t>(m_spinwaiter.m_u.rw.data, rw);
}

//------------------------------------------------------------------------------
/**
*/
void read_write_spin_wait::reader_access::lock() const noexcept
{
    auto const ticket_id =
        atomic_fetch_add_seq<uint16_t>(m_spinwaiter.m_u.split.my_tickets.data, 1);

    // Wait for out ticket
    while(ticket_id != atomic_fetch_acq(m_spinwaiter.m_u.split.my_reads.data)) 
    {}

    // Since we've entered a read section, allow other reads to continue.
    (void)atomic_fetch_add_seq<uint16_t>(m_spinwaiter.m_u.split.my_reads.data, 1);
}

//------------------------------------------------------------------------------
/**
*/
signalling_bool
read_write_spin_wait::reader_access::try_lock() const noexcept
{
    uint64_t value;
    auto const old = atomic_fetch_acq(m_spinwaiter.m_u.split_and_rw.data);

    do
    {
        auto reads = static_cast<uint16_t>(old);
        auto tickets = static_cast<uint16_t>(old >> 32);

        if(reads != tickets)
            return false;

        reads++;
        tickets++;
        value = (old & ~(mask << 16)) | (static_cast<uint64_t>(tickets) << 32) | reads;
    } while(!atomic_bcas_seq<uint64_t>(m_spinwaiter.m_u.split_and_rw.data, value, old));

    return true;
}

} // namespace xr::threading
//------------------------------------------------------------------------------