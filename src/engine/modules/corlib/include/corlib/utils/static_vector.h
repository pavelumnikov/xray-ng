// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/utils/type_traits.h"

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, utils)

//-----------------------------------------------------------------------------------------------------------
// A variable-size array container with fixed capacity.
template<class T, size_t Capacity, size_t Alignment = 16>
class static_vector
{
public:
    using value_type = T;
    using pointer = T*;
    using reference = T&;
    using const_reference = T const&;
    using size_type = size_t;

    static_vector();
    static_vector(size_type _count, const_reference defaultElement = T());
    ~static_vector();

    XR_DECLARE_DELETE_COPY_ASSIGNMENT(static_vector);

    const_reference operator[](size_type i) const;
    reference operator[](size_type i);

    void push_back(value_type&& val);

    template<typename ... Args>
    reference emplace_back(Args&&... args);

    size_type size() const;
    constexpr size_type capacity() const;

    bool is_empty() const;

    pointer begin();

private:
    static constexpr uint32_t Alignment_mask = (Alignment - 1);

    T* index_to_object(size_type index);
    void copy_ctor(T* element, const T& val);
    void call_move_ctor(T* element, T&& val);
    void call_dtor(T* element);

    size_type m_count;
    char m_raw_memory[sizeof(T) * Capacity + Alignment];
}; // class static_vector<T, Capacity, Alignment>

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<class T, size_t Capacity, size_t Alignment>
inline static_vector<T, Capacity, Alignment>::static_vector()
    : m_count(0)
{}

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<class T, size_t Capacity, size_t Alignment>
inline static_vector<T, Capacity, Alignment>::static_vector(size_type _count, const_reference defaultElement)
    : m_count { _count }
{
    XR_DEBUG_ASSERTION_MSG(m_count <= Capacity, "Too big size");
    for(size_type i = 0; i < m_count; i++)
        copy_ctor(begin() + i, defaultElement);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<class T, size_t Capacity, size_t Alignment>
inline static_vector<T, Capacity, Alignment>::~static_vector()
{
    for(size_type i = 0; i < m_count; i++)
        call_dtor(begin() + i);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<class T, size_t Capacity, size_t Alignment>
inline typename static_vector<T, Capacity, Alignment>::const_reference
static_vector<T, Capacity, Alignment>::operator[](size_type i) const
{
    XR_DEBUG_ASSERTION_MSG(i < size(), "bad index");
    return *index_to_object(i);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<class T, size_t Capacity, size_t Alignment>
inline typename static_vector<T, Capacity, Alignment>::reference
static_vector<T, Capacity, Alignment>::operator[](size_type i)
{
    XR_DEBUG_ASSERTION_MSG(i < size(), "bad index");
    return *index_to_object(i);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<class T, size_t Capacity, size_t Alignment>
inline void static_vector<T, Capacity, Alignment>::push_back(value_type&& val)
{
    XR_DEBUG_ASSERTION_MSG(m_count < Capacity, "Can't add element");
    call_move_ctor(index_to_object(m_count++), eastl::move(val));
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<class T, size_t Capacity, size_t Alignment>
template<typename ... Args>
inline typename static_vector<T, Capacity, Alignment>::reference
static_vector<T, Capacity, Alignment>::emplace_back(Args&&... args)
{
    XR_DEBUG_ASSERTION_MSG(m_count < Capacity, "Can't add element");
    // If value_type has a move constructor, it will use it and this operation may be faster than otherwise.
    return *::new((void*)index_to_object(m_count++)) T(eastl::forward<Args>(args)...);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<class T, size_t Capacity, size_t Alignment>
inline typename static_vector<T, Capacity, Alignment>::size_type
static_vector<T, Capacity, Alignment>::size() const
{
    return m_count;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<class T, size_t Capacity, size_t Alignment>
constexpr typename static_vector<T, Capacity, Alignment>::size_type
static_vector<T, Capacity, Alignment>::capacity() const
{
    return Capacity;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<class T, size_t Capacity, size_t Alignment>
inline bool static_vector<T, Capacity, Alignment>::is_empty() const
{
    return m_count == 0;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<class T, size_t Capacity, size_t Alignment>
inline typename static_vector<T, Capacity, Alignment>::pointer
static_vector<T, Capacity, Alignment>::begin()
{
    return index_to_object(0);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<class T, size_t Capacity, size_t Alignment>
inline typename static_vector<T, Capacity, Alignment>::pointer
static_vector<T, Capacity, Alignment>::index_to_object(size_type index)
{
    pbyte aligned_memory = (pbyte)(((uintptr_t)&m_raw_memory[0] + Alignment_mask) & ~(uintptr_t)Alignment_mask);
    T* obj = reinterpret_cast<T*>(aligned_memory + index * sizeof(T));
    return obj;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<class T, size_t Capacity, size_t Alignment>
inline void static_vector<T, Capacity, Alignment>::copy_ctor(T* element, const T& val)
{
    new(element) T(val);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<class T, size_t Capacity, size_t Alignment>
inline void static_vector<T, Capacity, Alignment>::call_move_ctor(T* element, T&& val)
{
    new(element) T(eastl::move(val));
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<class T, size_t Capacity, size_t Alignment>
inline void static_vector<T, Capacity, Alignment>::call_dtor(T* element)
{
    XR_UNREFERENCED_PARAMETER(element);
    element->~T();
}

XR_NAMESPACE_END(xr, utils)
//-----------------------------------------------------------------------------------------------------------