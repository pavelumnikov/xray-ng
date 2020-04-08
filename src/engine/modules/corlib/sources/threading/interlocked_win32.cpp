// This file is a part of xray-ng engine
//

#if !defined(XRAY_PLATFORM_WINDOWS)
#   error "This code is supported by Windows platform!"
#endif // !defined(XRAY_PLATFORM_WINDOWS)

#include "corlib/platform.h"
#include "corlib/types.h"
#include "../os_include_win32.h"
#include "corlib/etl/type_conversions.h"
#include <intrin.h>
#include <cassert>

// All exchanges

#pragma intrinsic(_InterlockedExchange8)
#pragma intrinsic(_InterlockedExchange16)
#pragma intrinsic(_InterlockedExchange)
#pragma intrinsic(_InterlockedExchange64)

// All exchange-adds

#pragma intrinsic(_InterlockedExchangeAdd8)
#pragma intrinsic(_InterlockedExchangeAdd16)
#pragma intrinsic(_InterlockedExchangeAdd)
#pragma intrinsic(_InterlockedExchangeAdd64)

// All compare exchanges

#pragma intrinsic(_InterlockedCompareExchange8)
#pragma intrinsic(_InterlockedCompareExchange16)
#pragma intrinsic(_InterlockedCompareExchange)
#pragma intrinsic(_InterlockedCompareExchange64)

//------------------------------------------------------------------------------
namespace xr::threading::internal
{

//------------------------------------------------------------------------------
/**
*/
int8_t __atomic_exchange(volatile int8_t* ptr, int8_t value) noexcept
{
    assert(ptr);

    auto const p = reinterpret_cast<volatile CHAR*>(ptr);
    auto const v = etl::implicit_cast<CHAR>(value);

    return etl::implicit_cast<int8_t>(_InterlockedExchange8(p, v));
}

//------------------------------------------------------------------------------
/**
*/
int16_t __atomic_exchange(volatile int16_t* ptr, int16_t value) noexcept
{
    assert(ptr);

    auto const p = reinterpret_cast<volatile SHORT*>(ptr);
    auto const v = etl::implicit_cast<SHORT>(value);

    return etl::implicit_cast<int16_t>(_InterlockedExchange16(p, v));
}

//------------------------------------------------------------------------------
/**
*/
int32_t __atomic_exchange(volatile int32_t* ptr, int32_t value) noexcept
{
    assert(ptr);

    auto const p = reinterpret_cast<volatile LONG*>(ptr);
    auto const v = etl::implicit_cast<LONG>(value);

    return etl::implicit_cast<int32_t>(_InterlockedExchange(p, v));
}

//------------------------------------------------------------------------------
/**
*/
int64_t __atomic_exchange(volatile int64_t* ptr, int64_t value) noexcept
{
    assert(ptr);

    auto const p = reinterpret_cast<volatile LONG64*>(ptr);
    auto const v = etl::implicit_cast<LONG64>(value);

    return etl::implicit_cast<int64_t>(_InterlockedExchange64(p, v));
}

//------------------------------------------------------------------------------
/**
*/
void* __atomic_exchange(void* volatile* ptr, void* value) noexcept
{
    return InterlockedExchangePointer(ptr, value);
}

//------------------------------------------------------------------------------
/**
*/
int8_t __atomic_exchange_add(volatile int8_t* ptr, int8_t value) noexcept
{
    assert(ptr);

    auto const p = reinterpret_cast<volatile CHAR*>(ptr);
    auto const v = etl::implicit_cast<CHAR>(value);

    return etl::implicit_cast<int8_t>(_InterlockedExchangeAdd8(p, v));
}

//------------------------------------------------------------------------------
/**
*/
int16_t __atomic_exchange_add(volatile int16_t* ptr, int16_t value) noexcept
{
    assert(ptr);

    auto const p = reinterpret_cast<volatile SHORT*>(ptr);
    auto const v = etl::implicit_cast<SHORT>(value);

    return etl::implicit_cast<int16_t>(_InterlockedExchangeAdd16(p, v));
}

//------------------------------------------------------------------------------
/**
*/
int32_t __atomic_exchange_add(volatile int32_t* ptr, int32_t value) noexcept
{
    assert(ptr);

    auto const p = reinterpret_cast<volatile LONG*>(ptr);
    auto const v = etl::implicit_cast<LONG>(value);

    return etl::implicit_cast<int32_t>(_InterlockedExchangeAdd(p, v));
}

//------------------------------------------------------------------------------
/**
*/
int64_t __atomic_exchange_add(volatile int64_t* ptr, int64_t value) noexcept
{
    assert(ptr);

    auto const p = reinterpret_cast<volatile LONG64*>(ptr);
    auto const v = etl::implicit_cast<LONG64>(value);

    return etl::implicit_cast<int64_t>(_InterlockedExchangeAdd64(p, v));
}

//------------------------------------------------------------------------------
/**
*/
int8_t __atomic_compare_exchange(volatile int8_t* ptr, int8_t value, int8_t comparand) noexcept
{
    assert(ptr);

    auto const p = reinterpret_cast<volatile CHAR*>(ptr);
    auto const v = etl::implicit_cast<CHAR>(value);
    auto const c = etl::implicit_cast<CHAR>(comparand);

    return etl::implicit_cast<int8_t>(_InterlockedCompareExchange8(p, v, c));
}

//------------------------------------------------------------------------------
/**
*/
int16_t __atomic_compare_exchange(volatile int16_t* ptr, int16_t value, int16_t comparand) noexcept
{
    assert(ptr);

    auto const p = reinterpret_cast<volatile SHORT*>(ptr);
    auto const v = etl::implicit_cast<SHORT>(value);
    auto const c = etl::implicit_cast<SHORT>(comparand);

    return etl::implicit_cast<int16_t>(_InterlockedCompareExchange16(p, v, c));
}

//------------------------------------------------------------------------------
/**
*/
int32_t __atomic_compare_exchange(volatile int32_t* ptr, int32_t value, int32_t comparand) noexcept
{
    assert(ptr);

    auto const p = reinterpret_cast<volatile LONG*>(ptr);
    auto const v = etl::implicit_cast<LONG>(value);
    auto const c = etl::implicit_cast<LONG>(comparand);

    return etl::implicit_cast<int32_t>(_InterlockedCompareExchange(p, v, c));
}

//------------------------------------------------------------------------------
/**
*/
int64_t __atomic_compare_exchange(volatile int64_t* ptr, int64_t value, int64_t comparand) noexcept
{
    assert(ptr);

    auto const p = reinterpret_cast<volatile LONG64*>(ptr);
    auto const v = etl::implicit_cast<LONG64>(value);
    auto const c = etl::implicit_cast<LONG64>(comparand);

    return etl::implicit_cast<int64_t>(_InterlockedCompareExchange64(p, v, c));
}

//------------------------------------------------------------------------------
/**
*/
void* __atomic_compare_exchange(void* volatile* ptr, void* value, void* comparand) noexcept
{
    return InterlockedCompareExchangePointer(ptr, value, comparand);
}

//------------------------------------------------------------------------------
/**
*/
int8_t __atomic_or_operation(volatile int8_t* ptr, int8_t value) noexcept
{
    assert(ptr);

    auto const p = reinterpret_cast<volatile CHAR*>(ptr);
    auto const v = etl::implicit_cast<CHAR>(value);

    return etl::implicit_cast<int8_t>(_InterlockedOr8(p, v));
}

//------------------------------------------------------------------------------
/**
*/
int16_t __atomic_or_operation(volatile int16_t* ptr, int16_t value) noexcept
{
    assert(ptr);

    auto const p = reinterpret_cast<volatile SHORT*>(ptr);
    auto const v = etl::implicit_cast<SHORT>(value);

    return etl::implicit_cast<int16_t>(_InterlockedOr16(p, v));
}

//------------------------------------------------------------------------------
/**
*/
int32_t __atomic_or_operation(volatile int32_t* ptr, int32_t value) noexcept
{
    assert(ptr);

    auto const p = reinterpret_cast<volatile LONG*>(ptr);
    auto const v = etl::implicit_cast<LONG>(value);

    return etl::implicit_cast<int32_t>(_InterlockedOr(p, v));
}

//------------------------------------------------------------------------------
/**
*/
int64_t __atomic_or_operation(volatile int64_t* ptr, int64_t value) noexcept
{
    assert(ptr);

    auto const p = reinterpret_cast<volatile LONG64*>(ptr);
    auto const v = etl::implicit_cast<LONG64>(value);

    return etl::implicit_cast<int64_t>(_InterlockedOr64(p, v));
}

//------------------------------------------------------------------------------
/**
*/
int8_t __atomic_and_operation(volatile int8_t* ptr, int8_t value) noexcept
{
    assert(ptr);

    auto const p = reinterpret_cast<volatile CHAR*>(ptr);
    auto const v = etl::implicit_cast<CHAR>(value);

    return etl::implicit_cast<int8_t>(_InterlockedAnd8(p, v));
}

//------------------------------------------------------------------------------
/**
*/
int16_t __atomic_and_operation(volatile int16_t* ptr, int16_t value) noexcept
{
    assert(ptr);

    auto const p = reinterpret_cast<volatile SHORT*>(ptr);
    auto const v = etl::implicit_cast<SHORT>(value);

    return etl::implicit_cast<int16_t>(_InterlockedAnd16(p, v));
}

//------------------------------------------------------------------------------
/**
*/
int32_t __atomic_and_operation(volatile int32_t* ptr, int32_t value) noexcept
{
    assert(ptr);

    auto const p = reinterpret_cast<volatile LONG*>(ptr);
    auto const v = etl::implicit_cast<LONG>(value);

    return etl::implicit_cast<int32_t>(_InterlockedAnd(p, v));
}

//------------------------------------------------------------------------------
/**
*/
int64_t __atomic_and_operation(volatile int64_t* ptr, int64_t value) noexcept
{
    assert(ptr);

    auto const p = reinterpret_cast<volatile LONG64*>(ptr);
    auto const v = etl::implicit_cast<LONG64>(value);

    return etl::implicit_cast<int64_t>(_InterlockedAnd64(p, v));
}

} // namespace xr::threading::internal
//------------------------------------------------------------------------------