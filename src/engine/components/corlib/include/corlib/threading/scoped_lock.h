// This file is a part of xray-ng engine
//

#pragma once

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, threading)

template< typename T >
class scoped_lock final
{
public:
    explicit scoped_lock(T& cs) XR_NOEXCEPT;
    ~scoped_lock() XR_NOEXCEPT;

    XR_DECLARE_DELETE_COPY_ASSIGNMENT(scoped_lock);
    XR_DECLARE_DELETE_MOVE_ASSIGNMENT(scoped_lock);

private:
    T& m_cs;
};

//-----------------------------------------------------------------------------------------------------------
/**
*/
template< typename T >
scoped_lock< T >::scoped_lock(T& cs) XR_NOEXCEPT
    : m_cs(cs)
{
    m_cs.lock();
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
template< typename T >
scoped_lock< T >::~scoped_lock() XR_NOEXCEPT
{
    m_cs.unlock();
}

XR_NAMESPACE_END(xr, threading)
//-----------------------------------------------------------------------------------------------------------
