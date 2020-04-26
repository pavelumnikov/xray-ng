// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/threading/interlocked.h"
#include "corlib/memory/memory_functions.h"
#include "corlib/memory/memory_allocator_base.h"

#define XR_TASK_ALLOCATOR_GRANULARITY 16

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, memory)

//-----------------------------------------------------------------------------------------------------------
template<size_t Size, size_t MaxCount>
class static_allocator : public base_allocator
{
public:
    static_allocator();
    virtual ~static_allocator() = default;

    XR_DECLARE_DELETE_COPY_ASSIGNMENT(static_allocator);
    XR_DECLARE_DELETE_MOVE_ASSIGNMENT(static_allocator);

    void initialize(uintptr_t allocator_stamp);
    bool check_all_free();

    virtual bool can_allocate_block(size_t const size) const XR_NOEXCEPT override;
    virtual size_t total_size() const XR_NOEXCEPT override;
    virtual size_t allocated_size() const XR_NOEXCEPT override;

protected:
    virtual pvoid call_malloc(size_t size
        XR_DEBUG_PARAMETERS_DESCRIPTION_DECLARATION
        XR_DEBUG_PARAMETERS_DECLARATION) override;

    virtual pvoid call_realloc(pvoid pointer, size_t new_size
        XR_DEBUG_PARAMETERS_DESCRIPTION_DECLARATION
        XR_DEBUG_PARAMETERS_DECLARATION) override;

    virtual void call_free(pvoid pointer
        XR_DEBUG_PARAMETERS_DECLARATION) override;

private:
    struct node_prefix
    {
        node_prefix* next;
        uintptr_t allocator_stamp;
    }; // struct node_prefix

    template<size_t granularity, size_t size>
    struct alignment
    {
        static XR_CONSTEXPR_CPP14_OR_CONST size_t value = ((size - 1) / granularity + 1) * granularity;
    }; // struct alignment<granularity, size>

    static XR_CONSTEXPR_CPP14_OR_CONST size_t max_count = MaxCount;
    static XR_CONSTEXPR_CPP14_OR_CONST size_t node_size = Size;
    static XR_CONSTEXPR_CPP14_OR_CONST size_t max_size = sizeof(node_prefix) + node_size;
    static XR_CONSTEXPR_CPP14_OR_CONST size_t granularity = alignment<XR_TASK_ALLOCATOR_GRANULARITY, max_size>::value;

    pvoid to_node(node_prefix* prefix)
    {
        return reinterpret_cast<pvoid>(prefix + 1);
    }

    node_prefix* from_node(pvoid n)
    {
        return reinterpret_cast<node_prefix*>(reinterpret_cast<pstr>(n) - node_size);
    }

    XR_ALIGNAS(XR_MAX_CACHE_LINE_SIZE) uint8_t m_buffer[max_count * granularity];

    union pointer_and_counter
    {
        volatile int64_t whole;

#if XRAY_PLATFORM_32BIT
        struct
        {
            uintptr_t pointer;
            uint32_t counter;
        };
#elif XRAY_PLATFORM_64BIT
        struct
        {
            uintptr_t pointer : 48;
            uint64_t counter : 16;
        };
#else
#   error "Unknown platform"
#endif // XRAY_PLATFORM_WIN32
    };

    pointer_and_counter m_free_list;
    uintptr_t m_allocator_stamp;
    threading::atomic_size_t m_total_size;
    threading::atomic_size_t m_allocated_size;
    bool m_is_valid;
}; // class static_allocator<Size, MaxCount>


//-----------------------------------------------------------------------------------------------------------
/**
 */
template<size_t Size, size_t MaxCount>
static_allocator<Size, MaxCount>::static_allocator()
    : m_buffer {}
    , m_allocator_stamp { 0 }
    , m_total_size { 0 }
    , m_allocated_size { 0 }
    , m_free_list { 0 }
    , m_is_valid { false }
{}

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<size_t Size, size_t MaxCount>
static_allocator<Size, MaxCount>::~static_allocator()
{
    XR_DEBUG_ASSERTION_MSG(check_all_free(), "some object references are still in use");
    m_is_valid = false;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<size_t Size, size_t MaxCount>
inline void
static_allocator<Size, MaxCount>::initialize(uintptr_t allocator_stamp)
{
    m_allocated_size = allocator_stamp;
    for(size_t i = 0; i < max_count; ++i)
    {
        node_prefix* current_node = reinterpret_cast<node_prefix*>(
            reinterpret_cast<pbyte>(m_buffer) + (i * max_size));
        node_prefix* next_node = reinterpret_cast<node_prefix*>(
            reinterpret_cast<pbyte>(m_buffer) + ((i + 1U) * max_size));

        current_node->allocator_stamp = m_allocator_stamp;
        current_node->next = (i == max_count - 1) ? nullptr : next_node;
    }

    m_free_list.pointer = reinterpret_cast<uintptr_t>(m_buffer);
    m_is_valid = true;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<size_t Size, size_t MaxCount>
bool static_allocator<Size, MaxCount>::check_all_free()
{
    XR_DEBUG_ASSERTION_MSG(m_is_valid, "allocator is not initialized");

    size_t free_count = 0U;
    node_prefix const* current_node =
        reinterpret_cast<node_prefix*>(m_free_list.pointer);

    while(current_node)
    {
        ++free_count;
        current_node = current_node->next;
    }

    return (free_count == max_count);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<size_t Size, size_t MaxCount>
inline pvoid
static_allocator<Size, MaxCount>::call_malloc(size_t size
    XR_DEBUG_PARAMETERS_DESCRIPTION_DECLARATION
    XR_DEBUG_PARAMETERS_DECLARATION)
{
    XR_DEBUG_ASSERTION_MSG(m_is_valid, "allocator is not initialized");
    XR_DEBUG_ASSERTION_MSG(can_allocate_block(size), "Could not fit object into pool");

    pointer_and_counter allocated_task;
    pointer_and_counter new_head;

    do
    {
        allocated_task = m_free_list;
        if(!allocated_task.pointer)
            return nullptr;

        new_head.pointer = reinterpret_cast<uintptr_t>(
            reinterpret_cast<node_prefix*>(allocated_task.pointer)->next);
        new_head.counter = allocated_task.counter + 1U;
    } while(!threading::atomic_bcas_seq(m_free_list.whole, new_head.whole, allocated_task.whole));

    return to_node(reinterpret_cast<node_prefix*>(allocated_task.pointer));
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<size_t Size, size_t MaxCount>
inline pvoid
static_allocator<Size, MaxCount>::call_realloc(pvoid pointer, size_t new_size
    XR_DEBUG_PARAMETERS_DESCRIPTION_DECLARATION
    XR_DEBUG_PARAMETERS_DECLARATION)
{
    XR_DEBUG_ASSERTION_MSG(m_is_valid, "allocator is not initialized");
    XR_DEBUG_ASSERTION_MSG(can_allocate_block(new_size), "Could not fit object into pool");
    if(!pointer)
        return call_malloc(new_size XR_DEBUG_PARAMETERS_DESCRIPTION XR_DEBUG_PARAMETERS);

    node_prefix* n = from_node(pointer);
    XR_DEBUG_ASSERTION_MSG(n->allocator_stamp == m_allocator_stamp,
        "invalid allocator for memory block");

    return pointer;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<size_t Size, size_t MaxCount>
void static_allocator<Size, MaxCount>::call_free(pvoid pointer
    XR_DEBUG_PARAMETERS_DECLARATION)
{
    XR_DEBUG_ASSERTION_MSG(m_is_valid, "allocator is not initialized");

    pointer_and_counter head;
    pointer_and_counter new_head;

    node_prefix* n = from_node(pointer);
    XR_DEBUG_ASSERTION_MSG(n->allocator_stamp == m_allocator_stamp,
        "invalid allocator for memory block");

    new_head.pointer = reinterpret_cast<uintptr_t>(n);

    do
    {
        head = m_free_list;

        reinterpret_cast<node_prefix*>(new_head.pointer)->next =
            reinterpret_cast<node_prefix*>(head.pointer);

        new_head.counter = head.counter;
    } while(!threading::atomic_bcas_seq(m_free_list.whole, new_head.whole, head.whole));
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<size_t Size, size_t MaxCount>
inline bool
static_allocator<Size, MaxCount>::can_allocate_block(size_t const size) const XR_NOEXCEPT
{
    return size <= node_size;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<size_t Size, size_t MaxCount>
inline size_t
static_allocator<Size, MaxCount>::total_size() const XR_NOEXCEPT
{
    return threading::atomic_fetch_acq(m_total_size);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<size_t Size, size_t MaxCount>
inline size_t
static_allocator<Size, MaxCount>::allocated_size() const XR_NOEXCEPT
{
    return threading::atomic_fetch_acq(m_allocated_size);
}

} // namespace az::tasks
//-----------------------------------------------------------------------------------------------------------
