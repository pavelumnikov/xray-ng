// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/etl/type_traits.h"

namespace xr::etl::containers
{
/// A variable-size array container with fixed capacity.
template<class T, size_t Capacity>
class static_vector
{
public:
    inline static_vector()
        : m_count { 0 }
    {}

    inline static_vector(size_t _count, const T& defaultElement = T())
        : m_count(_count)
    {
        XR_DEBUG_ASSERTION_MSG(m_count <= Capacity, "Too big size");
        for(size_t i = 0; i < m_count; i++)
            copy_ctor(begin() + i, defaultElement);
    }

    inline ~static_vector()
    {
        for(size_t i = 0; i < m_count; i++)
            call_dtor(begin() + i);
    }

    XR_DECLARE_DELETE_COPY_ASSIGNMENT(static_vector);

    inline const T& operator[](size_t i) const
    {
        XR_DEBUG_ASSERTION_MSG(i < size(), "bad index");
        return *index_to_object(i);
    }

    inline T& operator[](size_t i)
    {
        XR_DEBUG_ASSERTION_MSG(i < size(), "bad index");
        return *index_to_object(i);
    }

    inline void push_back(T&& val)
    {
        XR_DEBUG_ASSERTION_MSG(m_count < Capacity, "Can't add element");
        call_move_ctor(index_to_object(m_count++), std::move(val));
    }

    template<typename ... Args>
    inline T& emplace_back(Args&&... args)
    {
        XR_DEBUG_ASSERTION_MSG(m_count < Capacity, "Can't add element");
        return *::new((void*)index_to_object(m_count++)) T(std::forward<Args>(args)...); // If value_type has a move constructor, it will use it and this operation may be faster than otherwise.
    }

    inline size_t size() const
    {
        return m_count;
    }

    constexpr size_t capacity() const
    {
        return Capacity;
    }

    inline bool is_empty() const
    {
        return m_count == 0;
    }

    inline T* begin()
    {
        return index_to_object(0);
    }

private:
    static constexpr uint32_t Alignment = 16;
    static constexpr uint32_t Alignment_mask = (Alignment - 1);

    inline T* index_to_object(size_t index)
    {
        pbyte aligned_memory = (pbyte)(((uintptr_t)&m_raw_memory[0] + Alignment_mask) & ~(uintptr_t)Alignment_mask);
        T* obj = (T*)(aligned_memory + index * sizeof(T));
        return obj;
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

    size_t m_count;
    char m_raw_memory[sizeof(T) * Capacity + Alignment];
};

} // namespace xr::etl::containers