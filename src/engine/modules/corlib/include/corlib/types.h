// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/macro/noexcept.h"
#include "corlib/macro/constexpr.h"
#include "corlib/macro/namespaces.h"
#include "corlib/macro/assertion.h"
#include "corlib/macro/callconv.h"
#include "corlib/macro/interfacing.h"
#include "corlib/macro/unreferenced_parameter.h"

#define XR_KILOBYTES_TO_BYTES(_kb) ((_kb) << 10)
#define XR_MEGABYTES_TO_BYTES(_mb) ((_mb) << 20)
#define XR_GIGABYTES_TO_BYTES(_gb) ((_gb) << 30)

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, details)

//-----------------------------------------------------------------------------------------------------------
class signalling_bool_impl
{
public:
    XR_CONSTEXPR_CPP14_OR_INLINE signalling_bool_impl(bool const value) XR_NOEXCEPT;
    ~signalling_bool_impl();

    XR_CONSTEXPR_CPP14_OR_INLINE operator bool() const XR_NOEXCEPT;
    XR_CONSTEXPR_CPP14_OR_INLINE bool operator !() const XR_NOEXCEPT;

private:
    static XR_CONSTEXPR_CPP14_OR_CONST uint8_t unchecked_flag = 128;
    mutable uint8_t m_value;
}; // class signalling_bool_impl

//-----------------------------------------------------------------------------------------------------------
/**
 */
XR_CONSTEXPR_CPP14_OR_INLINE
signalling_bool_impl::signalling_bool_impl(bool const value) XR_NOEXCEPT
    : m_value((value ? uint8_t(1) : uint8_t(0)) | unchecked_flag)
{}

//-----------------------------------------------------------------------------------------------------------
/**
 */
inline signalling_bool_impl::~signalling_bool_impl()
{
    XR_DEBUG_ASSERTION_MSG((m_value & unchecked_flag) == 0,
        "signalling_bool was not checked! check the return value!");
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
XR_CONSTEXPR_CPP14_OR_INLINE
signalling_bool_impl::operator bool() const XR_NOEXCEPT
{
    m_value &= ~unchecked_flag;
    return (m_value != 0);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
XR_CONSTEXPR_CPP14_OR_INLINE bool
signalling_bool_impl::operator !() const XR_NOEXCEPT
{
    return !(bool)(*this);
}

XR_NAMESPACE_END(xr, details)
//-----------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr)

typedef void* pvoid;
typedef void const* pcvoid;
typedef char* pbyte;
typedef char const* pcbyte;
typedef char* pstr;
typedef char const* pcstr;
typedef wchar_t* pwstr;
typedef wchar_t const* pcwstr;

#ifdef MASTER_GOLD
typedef bool signalling_bool;
#else // #ifdef MASTER_GOLD
typedef details::signalling_bool_impl signalling_bool;
#endif // #ifdef MASTER_GOLD

typedef char string32[32];
typedef char string64[64];
typedef char string128[128];
typedef char string256[256];
typedef char string512[512];
typedef char string1024[1024];
typedef char string2048[2048];
typedef char string4096[4096];

XR_NAMESPACE_END(xr)
//-----------------------------------------------------------------------------------------------------------