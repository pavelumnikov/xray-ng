// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/platform.h"

//-----------------------------------------------------------------------------------------------------------
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

//-----------------------------------------------------------------------------------------------------------
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

//-----------------------------------------------------------------------------------------------------------
/**
*/
template< typename T >
scoped_managed_lock< T >::~scoped_managed_lock()
{
    if(!this->m_locking_pattern)
        this->m_cs.unlock();
}

} // namespace xr::threading
//-----------------------------------------------------------------------------------------------------------
