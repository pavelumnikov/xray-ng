// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/utils/type_traits.h"
#include "corlib/macro/assertion.h"

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, memory)

// forward declaration
class blob;

/**
 * @brief A non-owning reference to a buffer range.
 * @details
 * A buffer reference provides a range of utilities for describing a buffer - like
 * pointer to memory block where the buffer data resides.
 */
class buffer_range final
{
public:
    XR_CONSTEXPR_CPP14_OR_INLINE buffer_range();

    /**
     * @brief Creates a reference through a pointer and length.
     * @param pointer A pointer to memory location.
     * @param length Number of bytes in the buffer.
     */
    template <typename PointerType>
    XR_CONSTEXPR_CPP14_OR_INLINE buffer_range(PointerType* begin, PointerType* end);

    XR_CONSTEXPR_CPP14_OR_INLINE pvoid begin() const;
    XR_CONSTEXPR_CPP14_OR_INLINE pvoid end() const;

    XR_CONSTEXPR_CPP14_OR_INLINE pcvoid cbegin() const;
    XR_CONSTEXPR_CPP14_OR_INLINE pcvoid cend() const;

    bool is_valid() const;

private:
    pvoid m_begin;
    pvoid m_end;
}; // class buffer_range

//-----------------------------------------------------------------------------------------------------------
/**
 */
XR_CONSTEXPR_CPP14_OR_INLINE buffer_range::buffer_range()
    : m_begin(nullptr)
    , m_end(nullptr)
{}

//-----------------------------------------------------------------------------------------------------------
/**
 */
template <typename PointerType>
XR_CONSTEXPR_CPP14_OR_INLINE buffer_range::buffer_range(PointerType* begin, PointerType* end)
    : m_begin(reinterpret_cast<pvoid>(begin))
    , m_end(reinterpret_cast<pvoid>(end))
{}

//-----------------------------------------------------------------------------------------------------------
/**
 */
XR_CONSTEXPR_CPP14_OR_INLINE pvoid buffer_range::begin() const
{
    return m_begin;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
XR_CONSTEXPR_CPP14_OR_INLINE pvoid buffer_range::end() const
{
    return m_end;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
XR_CONSTEXPR_CPP14_OR_INLINE pcvoid buffer_range::cbegin() const
{
    return m_begin;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
XR_CONSTEXPR_CPP14_OR_INLINE pcvoid buffer_range::cend() const
{
    return m_end;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
inline bool buffer_range::is_valid() const
{
    return (m_begin != nullptr);
}

XR_NAMESPACE_END(xr, memory)
//-----------------------------------------------------------------------------------------------------------