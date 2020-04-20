// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/utils/type_traits.h"

//-----------------------------------------------------------------------------------------------------------
namespace xr::memory
{

// forward declaration
class blob;

/**
 * @brief A non-owning reference to a buffer.
 * @details
 * A buffer reference provides a range of utilities for describing a buffer - like
 * pointer to memory block where the buffer data resides and length of the data.
 * Additionally methods for accessing the data from the buffer are provided.
 */
class buffer_ref final
{
public:
    constexpr buffer_ref();

    /**
     * @brief Creates a reference through a pointer and length.
     * @param pointer A pointer to memory location.
     * @param length Number of bytes in the buffer.
     */
    template <typename PointerType>
    constexpr buffer_ref(PointerType* pointer, size_t const length);

    /**
     * @brief Creates a reference through const pointer and length.
     * @param pointer A pointer to memory block.
     * @param length Length of the block.
     */
    template <typename PointerType>
    constexpr buffer_ref(const PointerType* pointer, size_t const length);

    /**
     * @brief Obtains a reference to the underlying memory block treating it as
     * the given **reference_type**.
     * @return A reference to the underlying type instance.
     */
    template <typename ReferenceType>
    constexpr ReferenceType as();

    /**
     * @brief Obtains a reference to the underlying memory block treating it as a
     * representation of the given **pointer_type**.
     * @return A pointer to the underlying entity.
     */
    template <typename PointerType>
    constexpr PointerType as_pointer();

    /**
     * @brief Number of bytes in the buffer.
     */
    constexpr size_t length() const;

    /**
     * @brief Obtains a sub buffer with the given size.
     * @param length The size of sub buffer.
     * @return A sub buffer definition.
     */
    constexpr buffer_ref sub_buf(size_t const length) const;
    constexpr bool is_valid() const;

private:
    pvoid m_data;
    size_t m_size;
}; // class buffer_ref

//-----------------------------------------------------------------------------------------------------------
/**
 */
constexpr buffer_ref::buffer_ref()
    : m_data { nullptr }
    , m_size { 0 }
{}

//-----------------------------------------------------------------------------------------------------------
/**
 */
template <typename PointerType>
constexpr buffer_ref::buffer_ref(PointerType* pointer, size_t const length)
    : m_data(reinterpret_cast<void*>(pointer))
    , m_size(length)
{}

//-----------------------------------------------------------------------------------------------------------
/**
 */
template <typename PointerType>
constexpr buffer_ref::buffer_ref(const PointerType* pointer, size_t const length)
    : m_data(reinterpret_cast<void*>(const_cast<PointerType*>(pointer)))
    , m_size(length)
{}

//-----------------------------------------------------------------------------------------------------------
/**
 */
template <typename ReferenceType>
constexpr ReferenceType buffer_ref::as()
{
    static_assert(eastl::is_reference<ReferenceType>::value, "must be a reference type!");

    using base_type = typename eastl::remove_reference<ReferenceType>::type;
    using pointer_type = typename eastl::add_pointer<base_type>::type;

    return *reinterpret_cast<pointer_type>(m_data);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
template <typename PointerType>
constexpr PointerType buffer_ref::as_pointer()
{
    static_assert(eastl::is_pointer<PointerType>::value, "must be a pointer");
    return reinterpret_cast<PointerType>(m_data);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
constexpr size_t buffer_ref::length() const
{
    return m_size;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
constexpr buffer_ref buffer_ref::sub_buf(size_t const length) const
{
    return buffer_ref { m_data, length };
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
constexpr bool buffer_ref::is_valid() const
{
    return m_data != nullptr;
}

} // namespace xr::memory
//-----------------------------------------------------------------------------------------------------------