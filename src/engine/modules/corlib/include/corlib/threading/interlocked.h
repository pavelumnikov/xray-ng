// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/threading/atomic_types.h"
#include "corlib/utils/type_conversions.h"
#include "corlib/utils/type_inversions.h"
#include "EASTL/type_traits.h"

#if defined(XRAY_PLATFORM_WINDOWS)
/// Read-Write barrier intrinsics
extern "C" void _ReadWriteBarrier();
extern "C" void _WriteBarrier();
extern "C" void __faststorefence();
#pragma intrinsic(_ReadWriteBarrier)
#pragma intrinsic(_WriteBarrier)
#pragma intrinsic(__faststorefence)

#define XR_MEMORY_READWRITE_BARRIER _ReadWriteBarrier()
#define XR_MEMORY_WRITE_BARRIER _WriteBarrier()
#define XR_MEMORY_FULLCONSISTENCY_BARRIER __faststorefence()

#else
#define XR_MEMORY_READWRITE_BARRIER
#define XR_MEMORY_WRITE_BARRIER
#define XR_MEMORY_FULLCONSISTENCY_BARRIER

#endif

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, threading)

//-----------------------------------------------------------------------------------------------------------
namespace internal
{

int8_t __atomic_exchange(volatile int8_t* ptr, int8_t value) XR_NOEXCEPT;
int16_t __atomic_exchange(volatile int16_t* ptr, int16_t value) XR_NOEXCEPT;
int32_t __atomic_exchange(volatile int32_t* ptr, int32_t value) XR_NOEXCEPT;
int64_t __atomic_exchange(volatile int64_t* ptr, int64_t value) XR_NOEXCEPT;
void* __atomic_exchange(void* volatile* ptr, void* value) XR_NOEXCEPT;
int8_t __atomic_exchange_add(volatile int8_t* ptr, int8_t value) XR_NOEXCEPT;
int16_t __atomic_exchange_add(volatile int16_t* ptr, int16_t value) XR_NOEXCEPT;
int32_t __atomic_exchange_add(volatile int32_t* ptr, int32_t value) XR_NOEXCEPT;
int64_t __atomic_exchange_add(volatile int64_t* ptr, int64_t value) XR_NOEXCEPT;
int8_t __atomic_compare_exchange(volatile int8_t* ptr, int8_t value, int8_t comparand) XR_NOEXCEPT;
int16_t __atomic_compare_exchange(volatile int16_t* ptr, int16_t value, int16_t comparand) XR_NOEXCEPT;
int32_t __atomic_compare_exchange(volatile int32_t* ptr, int32_t value, int32_t comparand) XR_NOEXCEPT;
int64_t __atomic_compare_exchange(volatile int64_t* ptr, int64_t value, int64_t comparand) XR_NOEXCEPT;
void* __atomic_compare_exchange(void* volatile* ptr, void* value, void* comparand) XR_NOEXCEPT;
int8_t __atomic_or_operation(volatile int8_t* ptr, int8_t value) XR_NOEXCEPT;
int16_t __atomic_or_operation(volatile int16_t* ptr, int16_t value) XR_NOEXCEPT;
int32_t __atomic_or_operation(volatile int32_t* ptr, int32_t value) XR_NOEXCEPT;
int64_t __atomic_or_operation(volatile int64_t* ptr, int64_t value) XR_NOEXCEPT;
int8_t __atomic_and_operation(volatile int8_t* ptr, int8_t value) XR_NOEXCEPT;
int16_t __atomic_and_operation(volatile int16_t* ptr, int16_t value) XR_NOEXCEPT;
int32_t __atomic_and_operation(volatile int32_t* ptr, int32_t value) XR_NOEXCEPT;
int64_t __atomic_and_operation(volatile int64_t* ptr, int64_t value) XR_NOEXCEPT;

} // namespace internal
//-----------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------
enum class memory_order
{
    relaxed, //!< No order constraint, same as plain load/store. Unsafe but best performance
    acquire, //!< Must be a load op. Synchronize with a prior release in another thread.
    release, //!< Must be a store op. Synchronize with a later acquire in another thread.
    acquire_release, //!< Must be a load-modify-store op. Performs both acquire and release.
    sequential //!< Sequential consistency, safe total order but least performance
}; // enum class memory_order

// forward declarations
template< typename T, size_t size > struct interlocked_align;

//-----------------------------------------------------------------------------------------------------------
template< typename T > 
struct alignas(sizeof(int8_t)) interlocked_align< T, sizeof(int8_t) > final
{
    eastl::add_volatile_t<T> data;
};

//-----------------------------------------------------------------------------------------------------------
template< typename T >
struct alignas(sizeof(int16_t)) interlocked_align< T, sizeof(int16_t) > final
{
    eastl::add_volatile_t<T> data;
};

//-----------------------------------------------------------------------------------------------------------
template< typename T > 
struct alignas(sizeof(int32_t)) interlocked_align< T, sizeof(int32_t) > final
{
    eastl::add_volatile_t<T> data;
};

//-----------------------------------------------------------------------------------------------------------
template< typename T > 
struct alignas(sizeof(int64_t)) interlocked_align< T, sizeof(int64_t) > final
{
    eastl::add_volatile_t<T> data;
};

template< typename Type >
using interlocked_align_t = interlocked_align<Type, sizeof(Type)>;

//-----------------------------------------------------------------------------------------------------------
/**
*/
template< memory_order Order, typename T >
T atomic_fetch(volatile T& address) XR_NOEXCEPT
{
    auto constexpr is_valid_type = sizeof(T) <= 8;

    auto constexpr is_valid_order =
        Order != memory_order::release || Order != memory_order::acquire_release;

    static_assert(is_valid_type,
        "Atomic operation can be done only on 1-/2-/4-/8-byte sized objects");

    static_assert(is_valid_order,
        "Atomic fetching can be: Relaxed, Acquire and Sequential");

    static_assert(eastl::is_pointer_v<T> || eastl::is_trivial_v<T>);

    if constexpr(Order != memory_order::relaxed)
    {
        XR_MEMORY_READWRITE_BARRIER;
    }

    return address;
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
template<typename T>
auto atomic_fetch_relax(volatile T& address) XR_NOEXCEPT
{
    return atomic_fetch<memory_order::relaxed, T>(address);
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
template<typename T>
auto atomic_fetch_acq(volatile T& address) XR_NOEXCEPT
{
    return atomic_fetch<memory_order::acquire, T>(address);
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
template<typename T>
auto atomic_fetch_seq(volatile T& address) XR_NOEXCEPT
{
    return atomic_fetch<memory_order::sequential, T>(address);
}


//-----------------------------------------------------------------------------------------------------------
/**
*/
template< memory_order Order, typename T >
void atomic_store(volatile T& address, T value) XR_NOEXCEPT
{
    auto constexpr is_valid_type = (sizeof(T) <= 8);

    auto constexpr is_valid_order =
        Order != memory_order::acquire || Order != memory_order::acquire_release;

    static_assert(is_valid_type,
        "Atomic operation can be done only on 1-/2-/4-/8-byte sized objects");

    static_assert(is_valid_order,
        "Atomic storing can be: Relaxed, Release and Sequential");

    static_assert(eastl::is_pointer_v<T> || eastl::is_trivial_v<T>);

    auto constexpr is_release_order = 
        Order == memory_order::release;

    auto constexpr is_simple_order = 
        Order == memory_order::relaxed || is_release_order;

    if constexpr(is_simple_order)
    {
        address = static_cast<T>(value);
        if constexpr(is_release_order)
        {
            XR_MEMORY_READWRITE_BARRIER;
        }
    }
    else
    { // sequential consistency
        if constexpr(eastl::is_pointer_v<T>)
        {
            using pointer_type = eastl::add_pointer_t<eastl::add_volatile_t<void>>;
            using value_type = eastl::add_pointer_t<void>;

            (void)internal::__atomic_exchange(
                reinterpret_cast<pointer_type>(&address),
                utils::unsafe_type_cast<value_type, T>(value));
        }
        else if(eastl::is_trivial_v<T>)
        {
            if constexpr(sizeof(T) == sizeof(int8_t))
            {
                using pointer = eastl::add_pointer_t<eastl::add_volatile_t<int8_t>>;

                (void)internal::__atomic_exchange(
                    reinterpret_cast<pointer>(&address),
                    utils::unsafe_type_cast<int8_t, T>(value));
            }
            else if constexpr(sizeof(T) == sizeof(int16_t))
            {
                using pointer = eastl::add_pointer_t<eastl::add_volatile_t<int16_t>>;

                (void)internal::__atomic_exchange(
                    reinterpret_cast<pointer>(&address),
                    utils::unsafe_type_cast<int16_t, T>(value));
            }
            else if constexpr(sizeof(T) == sizeof(int32_t))
            {
                using pointer = eastl::add_pointer_t<eastl::add_volatile_t<int32_t>>;

                (void)internal::__atomic_exchange(
                    reinterpret_cast<pointer>(&address),
                    utils::unsafe_type_cast<int32_t, T>(value));
            }
            else if constexpr(sizeof(T) == sizeof(int64_t))
            {
                using pointer = eastl::add_pointer_t<eastl::add_volatile_t<int64_t>>;

                (void)internal::__atomic_exchange(
                    reinterpret_cast<pointer>(&address),
                    utils::unsafe_type_cast<int64_t, T>(value));
            }
        }
    }
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
template<typename T>
auto atomic_store_relax(volatile T& address, T value) XR_NOEXCEPT
{
    return atomic_store<memory_order::relaxed, T>(address, value);
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
template<typename T>
auto atomic_store_rel(volatile T& address, T value) XR_NOEXCEPT
{
    return atomic_store<memory_order::release, T>(address, value);
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
template<typename T>
auto atomic_store_seq(volatile T& address, T value) XR_NOEXCEPT
{
    return atomic_store<memory_order::sequential, T>(address, value);
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
template< memory_order Order, typename T >
T atomic_fetch_store(volatile T& address, T value) XR_NOEXCEPT
{
    auto constexpr is_valid_type = (sizeof(T) <= 8);

    auto constexpr is_valid_order =
        ((Order != memory_order::acquire) || (Order != memory_order::release));

    static_assert(is_valid_type,
        "Atomic operation can be done only on 1-/2-/4-/8-byte sized objects");

    static_assert(is_valid_order,
        "Atomic fetch-storing can be: Relaxed, Acquire-Release and Sequential");

    static_assert(eastl::is_pointer_v<T> || eastl::is_trivial_v<T>);

    using result = eastl::remove_cv_t<eastl::remove_reference_t<decltype(address)>>;
    result prev;

    auto constexpr is_acquire_release_order = (Order == memory_order::acquire_release);
    auto constexpr is_simple_order = ((Order == memory_order::relaxed) || is_acquire_release_order);

    if constexpr(is_simple_order)
    {
        prev = address;

        if constexpr(is_acquire_release_order)
        {
            XR_MEMORY_READWRITE_BARRIER;
        }

        address = static_cast<T>(value);

        if constexpr(is_acquire_release_order)
        {
            XR_MEMORY_WRITE_BARRIER;
        }
    }
    else
    { // sequential consistency
        if constexpr(sizeof(T) == sizeof(int8_t))
        {
            using pointer = eastl::add_pointer_t<eastl::add_volatile_t<int8_t>>;

            prev = (result)internal::__atomic_exchange(
                reinterpret_cast<pointer>(&address),
                utils::unsafe_type_cast<int8_t, T>(value));
        }
        else if constexpr(sizeof(T) == sizeof(int16_t))
        {
            using pointer = eastl::add_pointer_t<eastl::add_volatile_t<int16_t>>;

            prev = (result)internal::__atomic_exchange(
                reinterpret_cast<pointer>(&address),
                utils::unsafe_type_cast<int16_t, T>(value));
        }
        else if constexpr(sizeof(T) == sizeof(int32_t))
        {
            using pointer = eastl::add_pointer_t<eastl::add_volatile_t<int32_t>>;

            prev = (result)internal::__atomic_exchange(
                reinterpret_cast<pointer>(&address),
                utils::unsafe_type_cast<int32_t, T>(value));
        }
        else if constexpr(sizeof(T) == sizeof(int64_t))
        {
            using pointer = eastl::add_pointer_t<eastl::add_volatile_t<int64_t>>;

            prev = (result)internal::__atomic_exchange(
                reinterpret_cast<pointer>(&address),
                utils::unsafe_type_cast<int64_t, T>(value));
        }
    }

    return prev;
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
template<typename T>
auto atomic_fetch_store_relax(volatile T& address, T value) XR_NOEXCEPT
{
    return atomic_fetch_store<memory_order::relaxed, T>(address, value);
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
template<typename T>
auto atomic_fetch_store_acqrel(volatile T& address, T value) XR_NOEXCEPT
{
    return atomic_fetch_store<memory_order::acquire_release, T>(address, value);
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
template<typename T>
auto atomic_fetch_store_seq(volatile T& address, T value) XR_NOEXCEPT
{
    return atomic_fetch_store<memory_order::sequential, T>(address, value);
}


//-----------------------------------------------------------------------------------------------------------
/**
*/
template< memory_order Order, typename T >
T atomic_fetch_add(volatile T& address, T value) XR_NOEXCEPT
{
    auto constexpr is_valid_type = (sizeof(T) <= 8);

    static_assert(is_valid_type,
        "Atomic operation can be done only on 1-/2-/4-/8-byte sized objects");

    static_assert((Order != memory_order::acquire) || (Order != memory_order::release),
        "Atomic fetch-addition can be: relaxed, acquire_release and sequential");

    static_assert(eastl::is_pointer_v<T> || eastl::is_trivial_v<T>);

    using result = eastl::remove_cv_t<eastl::remove_reference_t<decltype(address)>>;
    result prev;

    auto constexpr is_acquire_release_order = (Order == memory_order::acquire_release);
    auto constexpr is_simple_order = ((Order == memory_order::relaxed) || is_acquire_release_order);

    if constexpr(is_simple_order)
    {
        prev = address;

        if constexpr(is_acquire_release_order)
        {
            XR_MEMORY_READWRITE_BARRIER;
        }

        address += static_cast<T>(value);

        if constexpr(is_acquire_release_order)
        {
            XR_MEMORY_WRITE_BARRIER;
        }
    }
    else
    {  // sequential consistency
        if constexpr(sizeof(T) == sizeof(int8_t))
        {
            using pointer = eastl::add_pointer_t<eastl::add_volatile_t<int8_t>>;

            prev = (result)internal::__atomic_exchange_add(
                reinterpret_cast<pointer>(&address),
                utils::unsafe_type_cast<int8_t, T>(value));
        }
        else if constexpr(sizeof(T) == sizeof(int16_t))
        {
            using pointer = eastl::add_pointer_t<eastl::add_volatile_t<int16_t>>;

            prev = (result)internal::__atomic_exchange_add(
                reinterpret_cast<pointer>(&address),
                utils::unsafe_type_cast<int16_t, T>(value));
        }
        else if constexpr(sizeof(T) == sizeof(int32_t))
        {
            using pointer = eastl::add_pointer_t<eastl::add_volatile_t<int32_t>>;

            prev = (result)internal::__atomic_exchange_add(
                reinterpret_cast<pointer>(&address),
                utils::unsafe_type_cast<int32_t, T>(value));
        }
        else if constexpr(sizeof(T) == sizeof(int64_t))
        {
            using pointer = eastl::add_pointer_t<eastl::add_volatile_t<int64_t>>;

            prev = (result)internal::__atomic_exchange_add(
                reinterpret_cast<pointer>(&address),
                utils::unsafe_type_cast<int64_t, T>(value));
        }
    }

    return prev;
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
template<typename T>
auto atomic_fetch_add_relax(volatile T& address, T value) XR_NOEXCEPT
{
    return atomic_fetch_add<memory_order::relaxed, T>(address, value);
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
template<typename T>
auto atomic_fetch_add_acqrel(volatile T& address, T value) XR_NOEXCEPT
{
    return atomic_fetch_add<memory_order::acquire_release, T>(address, value);
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
template<typename T>
auto atomic_fetch_add_seq(volatile T& address, T value) XR_NOEXCEPT
{
    return atomic_fetch_add<memory_order::sequential, T>(address, value);
}


//-----------------------------------------------------------------------------------------------------------
/**
*/
template<threading::memory_order order, typename T>
auto atomic_fetch_inc(volatile T& address) XR_NOEXCEPT
{
    return atomic_fetch_add<order, T>(address, 1) + 1;
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
template<typename T>
auto atomic_fetch_inc_relax(volatile T& address) XR_NOEXCEPT
{
    return atomic_fetch_inc<memory_order::relaxed, T>(address);
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
template<typename T>
auto atomic_fetch_inc_acqrel(volatile T& address) XR_NOEXCEPT
{
    return atomic_fetch_inc<memory_order::acquire_release, T>(address);
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
template<typename T>
auto atomic_fetch_inc_seq(volatile T& address) XR_NOEXCEPT
{
    return atomic_fetch_inc<memory_order::sequential, T>(address);
}


//-----------------------------------------------------------------------------------------------------------
/**
*/
template<memory_order Order, typename T>
auto atomic_add_fetch(volatile T& address, T value) XR_NOEXCEPT
{
    return atomic_fetch_add<Order, T>(address, value) + value;
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
template<typename T>
auto atomic_add_fetch_relax(volatile T& address, T value) XR_NOEXCEPT
{
    return atomic_add_fetch<memory_order::relaxed, T>(address, value);
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
template<typename T>
auto atomic_add_fetch_acqrel(volatile T& address, T value) XR_NOEXCEPT
{
    return atomic_add_fetch<memory_order::acquire_release, T>(address, value);
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
template<typename T>
auto atomic_add_fetch_seq(volatile T& address, T value) XR_NOEXCEPT
{
    return atomic_add_fetch<memory_order::sequential, T>(address, value);
}


//-----------------------------------------------------------------------------------------------------------
/**
*/
template<memory_order Order, typename T>
auto atomic_inc_fetch(volatile T& address) XR_NOEXCEPT
{
    return atomic_fetch_add<Order, T>(address, 1) + 1;
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
template<typename T>
auto atomic_inc_fetch_relax(volatile T& address) XR_NOEXCEPT
{
    return atomic_inc_fetch<memory_order::relaxed, T>(address);
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
template<typename T>
auto atomic_inc_fetch_acqrel(volatile T& address) XR_NOEXCEPT
{
    return atomic_inc_fetch<memory_order::acquire_release, T>(address);
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
template<typename T>
auto atomic_inc_fetch_seq(volatile T& address) XR_NOEXCEPT
{
    return atomic_inc_fetch<memory_order::sequential, T>(address);
}


//-----------------------------------------------------------------------------------------------------------
/**
*/
template< memory_order Order, typename T >
T atomic_fetch_sub(volatile T& address, T value) XR_NOEXCEPT
{
    auto constexpr is_valid_type = (sizeof(T) <= 8);

    static_assert(is_valid_type,
        "Atomic operation can be done only on 1-/2-/4-/8-byte sized objects");

    static_assert(Order != memory_order::acquire || Order != memory_order::release,
        "Atomic fetch-addition can be: relaxed, acquire_release and sequential");

    static_assert(eastl::is_pointer_v<T> || eastl::is_trivial_v<T>);

    using result = eastl::remove_cv_t<eastl::remove_reference_t<decltype(address)>>;
    result prev;

    auto constexpr is_acquire_release_order =
        Order == memory_order::acquire_release;

    auto constexpr is_simple_order = 
        Order == memory_order::relaxed || is_acquire_release_order;

    if constexpr(is_simple_order)
    {
        prev = address;

        if constexpr(is_acquire_release_order)
        {
            XR_MEMORY_READWRITE_BARRIER;
        }

        address -= static_cast<T>(value);

        if constexpr(is_acquire_release_order)
        {
            XR_MEMORY_WRITE_BARRIER;
        }
    }
    else
    {   // sequential consistency
        if constexpr(sizeof(T) == sizeof(int8_t))
        {
            using pointer = eastl::add_pointer_t<eastl::add_volatile_t<int8_t>>;

            prev = (result)internal::__atomic_exchange_add(
                reinterpret_cast<pointer>(&address),
                utils::implicit_cast<int8_t, utils::invert_if_unsigned_t<T>>((0 - value)));
        }
        else if constexpr(sizeof(T) == sizeof(int16_t))
        {
            using pointer = eastl::add_pointer_t<eastl::add_volatile_t<int16_t>>;

            prev = (result)internal::__atomic_exchange_add(
                reinterpret_cast<pointer>(&address),
                utils::implicit_cast<int16_t, utils::invert_if_unsigned_t<T>>((0 - value)));
        }
        else if constexpr(sizeof(T) == sizeof(int32_t))
        {
            using pointer = eastl::add_pointer_t<eastl::add_volatile_t<int32_t>>;

            prev = (result)internal::__atomic_exchange_add(
                reinterpret_cast<pointer>(&address),
                utils::implicit_cast<int32_t, utils::invert_if_unsigned_t<T>>((0 - value)));
        }
        else if constexpr(sizeof(T) == sizeof(int64_t))
        {
            using pointer = eastl::add_pointer_t<eastl::add_volatile_t<int64_t>>;

            prev = (result)internal::__atomic_exchange_add(
                reinterpret_cast<pointer>(&address),
                utils::implicit_cast<int64_t, utils::invert_if_unsigned_t<T>>((0 - value)));
        }
    }

    return prev;
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
template<typename T>
auto atomic_fetch_sub_relax(volatile T& address, T value) XR_NOEXCEPT
{
    return atomic_fetch_sub<memory_order::relaxed, T>(address, value);
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
template<typename T>
auto atomic_fetch_sub_acqrel(volatile T& address, T value) XR_NOEXCEPT
{
    return atomic_fetch_sub<memory_order::acquire_release, T>(address, value);
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
template<typename T>
auto atomic_fetch_sub_seq(volatile T& address, T value) XR_NOEXCEPT
{
    return atomic_fetch_sub<memory_order::sequential, T>(address, value);
}


//-----------------------------------------------------------------------------------------------------------
/**
*/
template<threading::memory_order order, typename T>
auto atomic_sub_fetch(volatile T& address, T value) XR_NOEXCEPT
{
    return atomic_fetch_sub<order, T>(address, value) - value;
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
template<typename T>
auto atomic_sub_fetch_relax(volatile T& address, T value) XR_NOEXCEPT
{
    return atomic_sub_fetch<memory_order::relaxed, T>(address, value);
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
template<typename T>
auto atomic_sub_fetch_acqrel(volatile T& address, T value) XR_NOEXCEPT
{
    return atomic_sub_fetch<memory_order::acquire_release, T>(address, value);
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
template<typename T>
auto atomic_sub_fetch_seq(volatile T& address, T value) XR_NOEXCEPT
{
    return atomic_sub_fetch<memory_order::sequential, T>(address, value);
}



//-----------------------------------------------------------------------------------------------------------
/**
*/
template<threading::memory_order order, typename T>
auto atomic_dec_fetch(volatile T& address) XR_NOEXCEPT
{
    return atomic_fetch_sub<order, T>(address, 1) - 1;
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
template<typename T>
auto atomic_dec_fetch_relax(volatile T& address) XR_NOEXCEPT
{
    return atomic_dec_fetch<memory_order::relaxed, T>(address);
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
template<typename T>
auto atomic_dec_fetch_acqrel(volatile T& address) XR_NOEXCEPT
{
    return atomic_dec_fetch<memory_order::acquire_release, T>(address);
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
template<typename T>
auto atomic_dec_fetch_seq(volatile T& address) XR_NOEXCEPT
{
    return atomic_dec_fetch<memory_order::sequential, T>(address);
}



//-----------------------------------------------------------------------------------------------------------
/**
*/
template< memory_order Order, typename T >
T atomic_cas(volatile T& address, T value, T comparand) XR_NOEXCEPT
{
    static_assert(sizeof(T) <= 8,
        "Atomic operation can be done only on 1-/2-/4-/8-byte sized objects");

    static_assert(Order != memory_order::acquire || Order != memory_order::release,
        "Atomic fetch-addition can be: Relaxed, Acquire-Release and Sequential");

    static_assert(eastl::is_pointer_v<T> || eastl::is_trivial_v<T>);

    using result = eastl::remove_volatile_t<eastl::remove_const_t<eastl::remove_reference_t<decltype(address)>>>;
    result prev;

    auto constexpr is_acquire_release_order = (Order == memory_order::acquire_release);
    auto constexpr is_simple_order = ((Order == memory_order::relaxed) || is_acquire_release_order);

    if constexpr(is_simple_order)
    {
        prev = address;

        if constexpr(is_acquire_release_order)
        {
            XR_MEMORY_READWRITE_BARRIER;
        }

        if(prev == static_cast<result>(comparand))
        {
            address = static_cast<result>(value);

            if constexpr(is_acquire_release_order)
            {
                XR_MEMORY_READWRITE_BARRIER;
            }
        }
    }
    else
    { // sequential consistency
        if constexpr(eastl::is_pointer_v<T>)
        {
            prev = (result)internal::__atomic_compare_exchange(
                reinterpret_cast<void* volatile*>(&address),
                utils::unsafe_type_cast<void*, T>(value),
                utils::unsafe_type_cast<void*, T>(comparand));
        }
        else if(eastl::is_trivial_v<T>)
        {
            if constexpr(sizeof(T) == sizeof(int8_t))
            {
                prev = (result)internal::__atomic_compare_exchange(
                    reinterpret_cast<int8_t volatile*>(&address),
                    utils::unsafe_type_cast<int8_t, T>(value),
                    utils::unsafe_type_cast<int8_t, T>(comparand));
            }
            else if constexpr(sizeof(T) == sizeof(int16_t))
            {
                prev = (result)internal::__atomic_compare_exchange(
                    reinterpret_cast<int16_t volatile*>(&address),
                    utils::unsafe_type_cast<int16_t, T>(value),
                    utils::unsafe_type_cast<int16_t, T>(comparand));
            }
            else if constexpr(sizeof(T) == sizeof(int32_t))
            {
                prev = (result)internal::__atomic_compare_exchange(
                    reinterpret_cast<int32_t volatile*>(&address),
                    utils::unsafe_type_cast<int32_t, T>(value),
                    utils::unsafe_type_cast<int32_t, T>(comparand));
            }
            else if constexpr(sizeof(T) == sizeof(int64_t))
            {
                prev = (result)internal::__atomic_compare_exchange(
                    reinterpret_cast<int64_t volatile*>(&address),
                    utils::unsafe_type_cast<int64_t, T>(value),
                    utils::unsafe_type_cast<int64_t, T>(comparand));
            }
        }
    }

    return prev;
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
template<typename T>
auto atomic_cas_relax(volatile T& address, T value, T comparand) XR_NOEXCEPT
{
    return atomic_cas<threading::memory_order::relaxed, T>(
        address, value, comparand);
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
template<typename T>
auto atomic_cas_acqrel(volatile T& address, T value, T comparand) XR_NOEXCEPT
{
    return atomic_cas<threading::memory_order::acquire_release, T>(
        address, value, comparand);
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
template<typename T>
auto atomic_cas_seq(volatile T& address, T value, T comparand) XR_NOEXCEPT
{
    return atomic_cas<threading::memory_order::sequential, T>(
        address, value, comparand);
}


//-----------------------------------------------------------------------------------------------------------
/**
*/
template< memory_order Order, typename T >
signalling_bool atomic_bcas(volatile T& address, T value, T comparand) XR_NOEXCEPT
{
    return atomic_cas<Order, T>(address, value, comparand) == static_cast<T>(comparand);
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
template<typename T>
auto atomic_bcas_relax(volatile T& address, T value, T comparand) XR_NOEXCEPT
{
    return atomic_bcas<threading::memory_order::relaxed, T>(
        address, value, comparand);
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
template<typename T>
auto atomic_bcas_acqrel(volatile T& address, T value, T comparand) XR_NOEXCEPT
{
    return atomic_bcas<threading::memory_order::acquire_release, T>(
        address, value, comparand);
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
template<typename T>
auto atomic_bcas_seq(volatile T& address, T value, T comparand) XR_NOEXCEPT
{
    return atomic_bcas<threading::memory_order::sequential, T>(
        address, value, comparand);
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
template<typename T >
T atomic_or(volatile T& address, T value) XR_NOEXCEPT
{
    static_assert(sizeof(T) <= 8,
        "Atomic operation can be done only on 1-/2-/4-/8-byte sized objects");

    static_assert(eastl::is_trivial_v<T>);
    using result = eastl::remove_volatile_t<eastl::remove_const_t<eastl::remove_reference_t<decltype(address)>>>;
    result prev;

    // TODO: add support for weak operations

    if constexpr(sizeof(T) == sizeof(int8_t))
    {
        prev = (result)internal::__atomic_or_operation(
            reinterpret_cast<int8_t volatile*>(&address),
            utils::unsafe_type_cast<int8_t, T>(value));
    }
    else if constexpr(sizeof(T) == sizeof(int16_t))
    {
        prev = (result)internal::__atomic_or_operation(
            reinterpret_cast<int16_t volatile*>(&address),
            utils::unsafe_type_cast<int16_t, T>(value));
    }
    else if constexpr(sizeof(T) == sizeof(int32_t))
    {
        prev = (result)internal::__atomic_or_operation(
            reinterpret_cast<int32_t volatile*>(&address),
            utils::unsafe_type_cast<int32_t, T>(value));
    }
    else if constexpr(sizeof(T) == sizeof(int64_t))
    {
        prev = (result)internal::__atomic_or_operation(
            reinterpret_cast<int64_t volatile*>(&address),
            utils::unsafe_type_cast<int64_t, T>(value));
    }

    return prev;
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
template<typename T >
T atomic_and(volatile T& address, T value) XR_NOEXCEPT
{
    static_assert(sizeof(T) <= 8,
        "Atomic operation can be done only on 1-/2-/4-/8-byte sized objects");

    static_assert(eastl::is_trivial_v<T>);
    using result = eastl::remove_volatile_t<eastl::remove_const_t<eastl::remove_reference_t<decltype(address)>>>;
    result prev;

    // TODO: add support for weak operations

    if constexpr(sizeof(T) == sizeof(int8_t))
    {
        prev = (result)internal::__atomic_and_operation(
            reinterpret_cast<int8_t volatile*>(&address),
            utils::unsafe_type_cast<int8_t, T>(value));
    }
    else if constexpr(sizeof(T) == sizeof(int16_t))
    {
        prev = (result)internal::__atomic_and_operation(
            reinterpret_cast<int16_t volatile*>(&address),
            utils::unsafe_type_cast<int16_t, T>(value));
    }
    else if constexpr(sizeof(T) == sizeof(int32_t))
    {
        prev = (result)internal::__atomic_and_operation(
            reinterpret_cast<int32_t volatile*>(&address),
            utils::unsafe_type_cast<int32_t, T>(value));
    }
    else if constexpr(sizeof(T) == sizeof(int64_t))
    {
        prev = (result)internal::__atomic_and_operation(
            reinterpret_cast<int64_t volatile*>(&address),
            utils::unsafe_type_cast<int64_t, T>(value));
    }

    return prev;
}

XR_NAMESPACE_END(xr, threading)
//-----------------------------------------------------------------------------------------------------------
