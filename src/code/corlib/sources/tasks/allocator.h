// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/threading/interlocked.h"
#include "corlib/memory/allocator_helper.h"
#include "corlib/memory/memory_functions.h"

#define XR_TASK_ALLOCATOR_GRANULARITY 16

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, tasks)

//-----------------------------------------------------------------------------------------------------------
template<typename Node, size_t MaxCount>
class allocator
{
public:
    using node_t = typename Node;

    allocator();
    ~allocator();

    XR_DECLARE_DELETE_COPY_ASSIGNMENT(allocator);
    XR_DECLARE_DELETE_MOVE_ASSIGNMENT(allocator);

    template<typename F>
    void construct_all_with(F&& functor);

    node_t* take_available();
    void put_back(node_t* p);
    bool check_all_free();

private:
    struct node_prefix
    {
        node_prefix* next;
    }; // struct node_prefix

    template<size_t granularity, size_t size>
    struct alignment
    {
        static constexpr size_t value = ((size - 1) / granularity + 1) * granularity;
    }; // struct alignment<granularity, size>

    static constexpr size_t max_count = MaxCount;
    static constexpr size_t node_size = sizeof(node_t);
    static constexpr size_t max_size = sizeof(node_prefix) + node_size;
    static constexpr size_t granularity = alignment<XR_TASK_ALLOCATOR_GRANULARITY, max_size>::value;

    node_t* to_node(node_prefix* prefix)
    {
        return reinterpret_cast<node_t*>(prefix + 1);
    }

    node_prefix* from_node(node_t* n)
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
}; // class allocator<Node, MaxCount>


//-----------------------------------------------------------------------------------------------------------
/**
 */
template<typename Node, size_t MaxCount>
allocator<Node, MaxCount>::allocator()
    : m_buffer {}
    , m_free_list { 0 }
{
    for(size_t i = 0; i < max_count; ++i)
    {
        auto current_node = reinterpret_cast<node_prefix*>(
            reinterpret_cast<pbyte>(m_buffer) + (i * max_size));
        auto next_node = reinterpret_cast<node_prefix*>(
            reinterpret_cast<pbyte>(m_buffer) + ((i + 1U) * max_size));

        current_node->next = (i == max_count - 1) ? nullptr : next_node;
    }

    m_free_list.pointer = reinterpret_cast<uintptr_t>(m_buffer);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<typename Node, size_t MaxCount>
allocator<Node, MaxCount>::~allocator()
{
    size_t free_count = 0U;
    node_prefix* current_node =
        reinterpret_cast<node_prefix*>(m_free_list.pointer);

    while(current_node)
    {
        auto n = to_node(current_node);
        memory::call_destruct(n);

        ++free_count;
        current_node = current_node->next;
    }

    XR_DEBUG_ASSERTION(free_count == max_count);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<typename Node, size_t MaxCount>
template<typename F>
inline void
allocator<Node, MaxCount>::construct_all_with(F&& functor)
{
    node_prefix* current_node =
        reinterpret_cast<node_prefix*>(m_free_list.pointer);

    while(current_node)
    {
        auto n = to_node(current_node);
        functor(n);
        current_node = current_node->next;
    }
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<typename Node, size_t MaxCount>
typename allocator<Node, MaxCount>::node_t*
allocator<Node, MaxCount>::take_available()
{
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
template<typename Node, size_t MaxCount>
void allocator<Node, MaxCount>::put_back(node_t* p)
{
    pointer_and_counter head;
    pointer_and_counter new_head;

    auto n = from_node(p);
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
template<typename Node, size_t MaxCount>
bool allocator<Node, MaxCount>::check_all_free()
{
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

XR_NAMESPACE_END(xr, tasks)
//-----------------------------------------------------------------------------------------------------------
