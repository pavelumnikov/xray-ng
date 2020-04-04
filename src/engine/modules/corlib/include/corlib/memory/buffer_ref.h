/*

  Copyright (c) 2019, Pavel Umnikov
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

#include "EASTL/type_traits.h"

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
    /**
     * @brief Creates a reference through a pointer and length.
     * @param pointer A pointer to memory location.
     * @param length Number of bytes in the buffer.
     */
    template <typename PointerType>
    constexpr buffer_ref(PointerType* pointer, size_t const length)
        : m_data(reinterpret_cast<void*>(pointer))
        , m_size(length)
    {}

    /**
     * @brief Creates a reference through const pointer and length.
     * @param pointer A pointer to memory block.
     * @param length Length of the block.
     */
    template <typename PointerType>
    constexpr buffer_ref(const PointerType* pointer, size_t const length)
        : m_data(reinterpret_cast<void*>(const_cast<PointerType*>(pointer)))
        , m_size(length)
    {}

    /**
     * @brief Obtains a reference to the underlying memory block treating it as
     * the given **reference_type**.
     * @return A reference to the underlying type instance.
     */
    template <typename ReferenceType>
    constexpr ReferenceType as()
    {
        static_assert(eastl::is_reference<ReferenceType>::value, "must be a reference type!");

        using base_type = typename eastl::remove_reference<ReferenceType>::type;
        using pointer_type = typename eastl::add_pointer<base_type>::type;

        return *reinterpret_cast<pointer_type>(m_data);
    }

    /**
     * @brief Obtains a reference to the underlying memory block treating it as a
     * representation of the given **pointer_type**.
     * @return A pointer to the underlying entity.
     */
    template <typename PointerType>
    constexpr PointerType as_pointer()
    {
        static_assert(eastl::is_pointer<PointerType>::value, "must be a pointer");
        return reinterpret_cast<PointerType>(m_data);
    }

    /**
     * @brief Number of bytes in the buffer.
     */
    constexpr size_t length() const
    {
        return m_size;
    }

    /**
     * @brief Obtains a sub buffer with the given size.
     * @param length The size of sub buffer.
     * @return A sub buffer definition.
     */
    constexpr buffer_ref sub_buf(size_t const length) const
    {
        return buffer_ref { m_data, length };
    }

    constexpr bool is_valid() const
    {
        return m_data != nullptr;
    }

private:
    void* m_data;
    size_t m_size;
}; // class buffer_ref

} // namespace xr::memory
