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

#include "corlib/memory/buffer_ref.h"
#include "corlib/memory/memory_allocator_base.h"
#include "EASTL/internal/move_help.h"

namespace xr::memory
{

class blob final
{
public:
    /// default constructor
    constexpr explicit blob(base_allocator& allocator) noexcept
        : m_ptr(nullptr)
        , m_size(0)
        , m_alloc_size(0)
        , m_allocator(&allocator)
    {}

    /// per-per byte copy constructor
    blob(base_allocator& allocator, buffer_ref& ref) noexcept(false)
        : blob { allocator }
    {
        this->self_copy(ref.as_pointer<void*>(), ref.length());
    }

    /// reserve N bytes
    blob(base_allocator& allocator, size_t const size) noexcept(false)
        : blob { allocator }
    {
        this->self_allocate(size);
    }

    /// copy constructor
    blob(const blob& rhs) noexcept(false)
        : blob { *rhs.m_allocator }
    {
        if(rhs.is_valid())
            this->self_copy(rhs.m_ptr, rhs.m_size);
    }

    /// move constructor
    blob(blob&& rhs) noexcept
        : m_ptr(eastl::move(rhs.m_ptr))
        , m_size(eastl::move(rhs.m_size))
        , m_alloc_size(eastl::move(rhs.m_alloc_size))
        , m_allocator(eastl::move(rhs.m_allocator))
    {
        rhs.m_ptr = nullptr;
        rhs.m_size = 0;
        rhs.m_alloc_size = 0;
    }

    /// destructor
    ~blob()
    {
        this->self_delete();
    }

    /// assignment operator
    blob& operator=(const blob& rhs) noexcept(false)
    {
        if(rhs.is_valid())
            this->self_copy(rhs.m_ptr, rhs.m_size);

        return *this;
    }

    /// move assignment operator
    blob& operator=(blob&& rhs) noexcept
    {
        this->m_ptr = eastl::move(rhs.m_ptr);
        rhs.m_ptr = nullptr;

        this->m_size = eastl::move(rhs.m_size);
        rhs.m_size = 0;

        this->m_alloc_size = eastl::move(rhs.m_alloc_size);
        rhs.m_alloc_size = 0;

        this->m_allocator = eastl::move(rhs.m_allocator);

        return *this;
    }

    /// equality operator
    bool operator==(const blob& rhs) const
    {
        return (this->binary_compare(rhs) == 0);
    }

    /// inequality operator
    bool operator!=(const blob& rhs) const
    {
        return (this->binary_compare(rhs) != 0);
    }

    /// greater operator
    bool operator>(const blob& rhs) const
    {
        return (this->binary_compare(rhs) > 0);
    }

    /// less operator
    bool operator<(const blob& rhs) const
    {
        return (this->binary_compare(rhs) < 0);
    }

    /// greater-equal operator
    bool operator>=(const blob& rhs) const
    {
        return (this->binary_compare(rhs) >= 0);
    }

    /// less-equal operator
    bool operator<=(const blob& rhs) const
    {
        return (this->binary_compare(rhs) <= 0);
    }

    /// return true if the blob contains data
    constexpr bool is_valid() const
    {
        auto const result = this->get_ptr_unsafe();
        return result != nullptr;
    }

    /// reserve N bytes
    void reserve(size_t const size)
    {
        auto const alloc_size = this->get_alloc_size_unsafe();

        if(alloc_size < size)
        {
            this->self_delete();
            this->self_allocate(size);
        }

        this->set_size_unsafe(size);
    }

    /// trim the size member (without re-allocating!)
    void trim(size_t const size)
    {
        this->set_size_unsafe(size);
    }

    /// set blob contents
    void set(buffer_ref& ref)
    {
        this->self_copy(ref.as_pointer<void*>(), ref.length());
    }

    /// get blob size
    constexpr buffer_ref get() const
    {
        return buffer_ref { this->m_ptr, this->m_size };
    }

    /// get a hash code (compatible with Util::HashTable)
    size_t hash_code() const;

private:
    /// delete content
    void self_delete();
    /// allocate internal buffer
    void self_allocate(size_t size);
    /// copy content
    void self_copy(const void* ptr, size_t size);
    /// do a binary comparison between this and other blob
    int binary_compare(const blob& rhs) const;

    constexpr void set_size_unsafe(size_t const sz)
    {
        m_size = sz;
    }

    constexpr void set_size_and_ptr_unsafe(void* ptr, size_t const sz)
    {
        m_size = sz;
        m_ptr = ptr;
    }

    constexpr void set_size_and_ptr_unsafe(void* ptr, size_t const sz, size_t const alloc)
    {
        m_size = sz;
        m_ptr = ptr;
        m_alloc_size = alloc;
    }

    constexpr void* get_ptr_unsafe() const
    {
        auto const result = this->m_ptr;
        return result;
    }

    constexpr size_t get_size_unsafe() const
    {
        auto const result = this->m_size;
        return result;
    }

    constexpr size_t get_alloc_size_unsafe() const
    {
        auto const result = this->m_alloc_size;
        return result;
    }

    void* m_ptr;
    size_t m_size;
    size_t m_alloc_size;
    base_allocator* m_allocator;
}; // class blob

} // namespace aztek::core::memory

namespace eastl
{

using namespace xr;
template <typename T> struct hash;

template<>
struct hash<memory::blob>
{
    size_t operator()(const memory::blob& rhs) const
    {
        return rhs.hash_code();
    }
};

} // namespace eastl