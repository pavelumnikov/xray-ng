// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/sys/win/min_windows.h"

namespace xr::sys::win
{

class handle_traits final
{
public:
    using type = HANDLE;

    static bool close(_In_ type h) noexcept;
    static HANDLE get_invalid_value() noexcept;
};

// Handle object implementation specialized with traits
template<typename HandleTraits>
class handle_wrapper final
{
public:
    using traits = HandleTraits;
    using type = typename traits::type;

    // No new operator
    void* operator new(size_t) = delete;
    // No delete operator
    void operator delete(void *, size_t) = delete;

    explicit handle_wrapper(type h = traits::get_invalid_value()) noexcept
        : m_handle(h)
    {}

    handle_wrapper(_Inout_ handle_wrapper&& h) noexcept
        : m_handle(h.m_handle)
    {
        h.m_handle = traits::get_invalid_value();
    }

    handle_wrapper(const handle_wrapper&) = delete;
    handle_wrapper& operator=(const handle_wrapper&) = delete;

    ~handle_wrapper() noexcept
    {
        close();
    }

    handle_wrapper& operator=(_Inout_ handle_wrapper&& h) noexcept
    {
        close();
        this->m_handle = h.m_handle;
        h.m_handle = traits::get_invalid_value();
        return *this;
    }

    void attach(type h) noexcept
    {
        if(h != this->m_handle)
        {
            close();
            this->m_handle = h;
        }
    }

    type detach()
    {
        type h = this->m_handle;
        this->m_handle = traits::get_invalid_value();
        return h;
    }

    type get() const
    {
        return this->m_handle;
    }

    void close() noexcept
    {
        if(this->m_handle != traits::get_invalid_value())
        {
            auto const ret = internal_close();
            if(!ret)
            {
                // If we cannot close the handle it indicates 
                // bug in somebody code thus we raise an exception
                //::Microsoft::WRL::Details::RaiseException( HRESULT_FROM_WIN32( GetLastError() ) );
            }

            this->m_handle = traits::get_invalid_value();
        }
    }

    bool is_valid() const noexcept
    {
        return this->m_handle != traits::get_invalid_value();
    }

    type* get_address_of() noexcept
    {
        return &this->m_handle;
    }

    type* release_and_get_address_of() noexcept
    {
        close();
        return &this->m_handle;
    }

protected:
    bool internal_close() const noexcept
    {
        return traits::close(this->m_handle);
    }

    type m_handle;
};

// HandleT comparison operators
template<class T>
bool operator==(const handle_wrapper<T>& rhs, const handle_wrapper<T>& lhs) noexcept
{
    return rhs.get() == lhs.get();
}

template<class T>
bool operator==(const typename handle_wrapper<T>::type& lhs, const handle_wrapper<T>& rhs) noexcept
{
    return lhs == rhs.get();
}

template<class T>
bool operator==(const handle_wrapper<T>& lhs, const typename handle_wrapper<T>::type& rhs) noexcept
{
    return lhs.get() == rhs;
}

template<class T>
bool operator!=(const handle_wrapper<T>& lhs, const handle_wrapper<T>& rhs) noexcept
{
    return lhs.get() != rhs.get();
}

template<class T>
bool operator!=(const typename handle_wrapper<T>::type& lhs, const handle_wrapper<T>& rhs) noexcept
{
    return lhs != rhs.get();
}

template<class T>
bool operator!=(const handle_wrapper<T>& lhs, const typename handle_wrapper<T>::type& rhs) noexcept
{
    return lhs.get() != rhs;
}

template<class T>
bool operator<(const handle_wrapper<T>& lhs, const handle_wrapper<T>& rhs) noexcept
{
    return lhs.get() < rhs.get();
}

using file_handle_wrapper = handle_wrapper<handle_traits>;

} // namespace xr::sys::win
