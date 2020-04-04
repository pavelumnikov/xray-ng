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

#include "corlib/platform.h"

//------------------------------------------------------------------------------
namespace xr::threading
{

/// <summary>
/// <c>scoped_managed_lock</c> is a RAII class that allows to do manual locking of some section. Can be
/// used in situations where algorithm can select the best way to produce results using locking results.
/// </summary>
template< typename T >
class scoped_managed_lock final
{
public:
    /// <summary>
    /// </summary>
    /// <param name="cs">Reference to a locking object.</param>
    /// <param name="force_lock">Lock by default on true, TryLock on false.</param>
    /// <param name="is_scope_locked">Retrieves result of locking.</param>
    scoped_managed_lock(T& cs, bool force_lock, bool& is_scope_locked) noexcept;

    /// <summary>
    /// Unlocks locking object.
    /// </summary>
    ~scoped_managed_lock();

    scoped_managed_lock(const scoped_managed_lock&) = delete;
    scoped_managed_lock& operator=(const scoped_managed_lock&) = delete;

    scoped_managed_lock(scoped_managed_lock&&) = delete;
    scoped_managed_lock& operator=(scoped_managed_lock&&) = delete;

private:
    uint8_t m_locking_pattern;
    T& m_cs;
};

//------------------------------------------------------------------------------
/**
*/
template< typename T >
scoped_managed_lock< T >::scoped_managed_lock(T& cs, bool const force_lock, bool& is_scope_locked) noexcept
    : m_locking_pattern(1)
    , m_cs(cs)
{
    if(force_lock)
    {
        this->m_cs.lock();
        is_scope_locked = true;
    }
    else
    {
        if(this->m_cs.try_lock())
        {
            this->m_locking_pattern = 0;
            is_scope_locked = true;
        }
        else
        {
            is_scope_locked = false;
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
template< typename T >
scoped_managed_lock< T >::~scoped_managed_lock()
{
    if(!this->m_locking_pattern)
        this->m_cs.unlock();
}

} // namespace xr::threading
//------------------------------------------------------------------------------