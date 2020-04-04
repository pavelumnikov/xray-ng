/*

  Copyright (c) 2020, Pavel Umnikov
  All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:

  * Redistributions of source code must retain the above copyright notice,
    this list of conditions and the following disclaimer.
  * Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.

  THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND ANY
  EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  DISCLAIMED. IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE FOR ANY
  DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
  OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
  DAMAGE.

*/

#pragma once

#include "corlib/etl/containers/std/vector.h"
#include "corlib/threading/spin_wait.h"
#include "corlib/threading/scoped_lock.h"
#include "corlib/memory/memory_aligned_allocator.h"

namespace xr::tasks
{

template<typename T, size_t Priority, size_t Capacity>
class priority_task_queue
{
    //////////////////////////////////////////////////////////////////////////
    class queue
    {
        static const uint32_t ALIGNMENT = 16;
        static const uint32_t MASK = Capacity - 1u;

        void* m_data;
        size_t begin;
        size_t end;

        inline T* buffer()
        {
            return (T*)(m_data);
        }

        inline void copy_ctor(T* element, const T& val)
        {
            new(element) T(val);
        }

        inline void call_move_ctor(T* element, T&& val)
        {
            new(element) T(std::move(val));
        }

        inline void call_dtor(T* element)
        {
            XR_UNREFERENCED_PARAMETER(element);
            element->~T();
        }

        inline size_t size() const
        {
            if(is_empty())
            {
                return 0;
            }

            size_t count = ((end & MASK) - (begin & MASK)) & MASK;
            return count;
        }

        inline void clear()
        {
            size_t queueSize = size();
            for(size_t i = 0; i < queueSize; i++)
            {
                T* pElement = buffer() + ((begin + i) & MASK);
                call_dtor(pElement);
            }

            begin = 0;
            end = 0;
        }

    public:

        queue()
            : m_data { nullptr }
            , begin { 0 }
            , end { 0 }
        {}

        // queue is just dummy until you call the Create
        void create(memory::base_allocator& alloc)
        {
            size_t bytesCount = sizeof(T) * Capacity;
            m_data = XR_ALLOCATE_MEMORY(alloc, bytesCount, "concurrent queue: queue");
        }

        void destroy(memory::base_allocator& alloc)
        {
            XR_DEALLOCATE_MEMORY(alloc, m_data);
            m_data = nullptr;
        }

        ~queue()
        {
            XR_DEBUG_ASSERTION(m_data == nullptr);
        }

        inline bool has_space(size_t itemCount)
        {
            if((size() + itemCount) >= Capacity)
            {
                return false;
            }

            return true;
        }

        inline bool add(const T& item)
        {
            XR_DEBUG_ASSERTION_MSG(m_data, "Can't add items to dummy queue");

            if((size() + 1) >= Capacity)
            {
                return false;
            }

            size_t index = (end & MASK);
            T* pElement = buffer() + index;
            copy_ctor(pElement, item);
            end++;

            return true;
        }


        inline bool try_pop_oldest(T& item)
        {
            if(is_empty())
                return false;

            XR_DEBUG_ASSERTION_MSG(m_data, "Can't pop items from dummy queue");

            size_t index = (begin & MASK);
            T* pElement = buffer() + index;
            begin++;
            item = *pElement;
            call_dtor(pElement);
            return true;
        }

        inline bool try_pop_newest(T& item)
        {
            if(is_empty())
                return false;

            XR_DEBUG_ASSERTION_MSG(m_data, "Can't pop items from dummy queue");

            end--;
            size_t index = (end & MASK);
            T* pElement = buffer() + index;
            item = *pElement;
            call_dtor(pElement);
            return true;
        }

        inline bool is_empty() const
        {
            return (begin == end);
        }
    };
    //////////////////////////////////////////////////////////////////////////

    using mutex = threading::spin_wait_fairness;
    mutex m_mutex;
    queue m_queues[Priority];

public:
    priority_task_queue();
    ~priority_task_queue();

    void initialize_memory_pool(memory::base_allocator& alloc);
    void shutdown_memory_pool(memory::base_allocator& alloc);

    XR_DECLARE_DELETE_COPY_ASSIGNMENT(priority_task_queue);
    XR_DECLARE_DELETE_MOVE_ASSIGNMENT(priority_task_queue);

    bool add(const T* item_array, size_t count);
    bool try_pop_oldest(T& item);
    bool try_pop_newest(T& item);
}; // class priority_task_queue<T, Priority, Capacity>

//------------------------------------------------------------------------------
/**
 */
template<typename T, size_t Priority, size_t Capacity>
inline priority_task_queue<T, Priority, Capacity>::priority_task_queue()
{}

//------------------------------------------------------------------------------
/**
 */
template<typename T, size_t Priority, size_t Capacity>
inline void 
priority_task_queue<T, Priority, Capacity>::initialize_memory_pool(memory::base_allocator& alloc)
{
    for(uint32_t i = 0; i < eastl::size(m_queues); i++)
        m_queues[i].create(alloc);
}

//------------------------------------------------------------------------------
/**
 */
template<typename T, size_t Priority, size_t Capacity>
inline void
priority_task_queue<T, Priority, Capacity>::shutdown_memory_pool(memory::base_allocator& alloc)
{
    for(uint32_t i = 0; i < eastl::size(m_queues); i++)
        m_queues[i].destroy(alloc);
}

//------------------------------------------------------------------------------
/**
 */
template<typename T, size_t Priority, size_t Capacity>
inline priority_task_queue<T, Priority, Capacity>::~priority_task_queue()
{}

//------------------------------------------------------------------------------
/**
 */
template<typename T, size_t Priority, size_t Capacity>
inline bool
priority_task_queue<T, Priority, Capacity>::add(const T* item_array, size_t count)
{
    threading::scoped_lock lock { m_mutex };
    // Check for space for all m_queues.
    // At the moment it is not known exactly in what queue items will be added.
    for(size_t i = 0; i < eastl::size(m_queues); i++)
    {
        queue& q = m_queues[i];
        if(!q.has_space(count))
            return false;
    }

    // Adding the tasks into the appropriate queue
    for(size_t i = 0; i < count; i++)
    {
        const T& item = item_array[i];
        uint32_t queueIndex = (uint32_t)item.desc.priority;
        XR_DEBUG_ASSERTION_MSG(queueIndex < eastl::size(m_queues), "Invalid task priority");

        queue& q = m_queues[queueIndex];
        bool res = q.add(item_array[i]);
        XR_UNREFERENCED_PARAMETER(res);
        XR_DEBUG_ASSERTION_MSG(res == true, "Sanity check failed");
    }

    return true;
}

//------------------------------------------------------------------------------
/**
 */
template<typename T, size_t Priority, size_t Capacity>
inline bool
priority_task_queue<T, Priority, Capacity>::try_pop_oldest(T& item)
{
    threading::scoped_lock lock { m_mutex };
    for(uint32_t queueIndex = 0; queueIndex < 3; queueIndex++)
    {
        queue& q = m_queues[queueIndex];
        if(q.try_pop_oldest(item))
            return true;
    }

    return false;
}

//------------------------------------------------------------------------------
/**
 */
template<typename T, size_t Priority, size_t Capacity>
inline bool
priority_task_queue<T, Priority, Capacity>::try_pop_newest(T& item)
{
    threading::scoped_lock lock { m_mutex };
    for(uint32_t queueIndex = 0; queueIndex < 3; queueIndex++)
    {
        queue& queue = m_queues[queueIndex];
        if(queue.try_pop_newest(item))
            return true;
    }
    return false;
}

} // namespace xr::tasks
//------------------------------------------------------------------------------