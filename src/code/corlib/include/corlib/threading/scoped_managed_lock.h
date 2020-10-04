// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/types.h"

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, threading)

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
    scoped_managed_lock(T& cs, bool force_lock, bool& is_scope_locked) XR_NOEXCEPT;

    /// <summary>
    /// Unlocks locking object.
    /// </summary>
    ~scoped_managed_lock();

    XR_DECLARE_DELETE_COPY_ASSIGNMENT(scoped_managed_lock);
    XR_DECLARE_DELETE_MOVE_ASSIGNMENT(scoped_managed_lock);

private:
    uint8_t m_locking_pattern;
    T& m_cs;
};

//-----------------------------------------------------------------------------------------------------------
/**
*/
template< typename T >
scoped_managed_lock< T >::scoped_managed_lock(T& cs, bool const force_lock, bool& is_scope_locked) XR_NOEXCEPT
    : m_locking_pattern(1)
    , m_cs(cs)
{
    if(force_lock)
    {
        m_cs.lock();
        is_scope_locked = true;
    }
    else
    {
        if(m_cs.try_lock())
        {
            m_locking_pattern = 0;
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
    if(!m_locking_pattern)
        m_cs.unlock();
}

XR_NAMESPACE_END(xr, threading)
//-----------------------------------------------------------------------------------------------------------
