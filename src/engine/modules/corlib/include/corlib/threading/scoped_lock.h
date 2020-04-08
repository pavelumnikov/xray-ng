// This file is a part of xray-ng engine
//

#pragma once

//------------------------------------------------------------------------------
namespace xr::threading
{

template< typename T >
class scoped_lock final
{
public:
    explicit scoped_lock(T& cs) noexcept;
    ~scoped_lock() noexcept;

    scoped_lock(const scoped_lock&) = delete;
    scoped_lock& operator=(const scoped_lock&) = delete;

    scoped_lock(scoped_lock&&) = delete;
    scoped_lock& operator=(scoped_lock&&) = delete;

private:
    T& m_cs;
};

//------------------------------------------------------------------------------
/**
*/
template< typename T >
scoped_lock< T >::scoped_lock(T& cs) noexcept
    : m_cs(cs)
{
    this->m_cs.lock();
}

//------------------------------------------------------------------------------
/**
*/
template< typename T >
scoped_lock< T >::~scoped_lock() noexcept
{
    this->m_cs.unlock();
}

} // namespace xr::threading
//------------------------------------------------------------------------------