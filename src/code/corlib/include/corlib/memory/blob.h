// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/memory/buffer_ref.h"
#include "corlib/memory/memory_allocator_base.h"
#include "EASTL/internal/move_help.h"

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, memory)

//-----------------------------------------------------------------------------------------------------------
class blob final
{
public:
    /// default constructor
    XR_CONSTEXPR_CPP14_OR_INLINE explicit blob(base_allocator& allocator) XR_NOEXCEPT
        : m_ptr(nullptr)
        , m_size(0)
        , m_alloc_size(0)
        , m_allocator(&allocator)
    {}

    /// per-per byte copy constructor
    blob(base_allocator& allocator, buffer_ref& ref) XR_NOEXCEPT
        : blob { allocator }
    {
        self_copy(ref.as_pointer<void*>(), ref.length());
    }

    /// reserve N bytes
    blob(base_allocator& allocator, size_t const size) XR_NOEXCEPT
        : blob { allocator }
    {
        self_allocate(size);
    }

    /// copy constructor
    blob(const blob& rhs) XR_NOEXCEPT
        : blob { *rhs.m_allocator }
    {
        if(rhs.is_valid())
            self_copy(rhs.m_ptr, rhs.m_size);
    }

    /// move constructor
    blob(blob&& rhs) XR_NOEXCEPT
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
        self_delete();
    }

    /// assignment operator
    blob& operator=(const blob& rhs) XR_NOEXCEPT
    {
        if(rhs.is_valid())
            self_copy(rhs.m_ptr, rhs.m_size);

        return *this;
    }

    /// move assignment operator
    blob& operator=(blob&& rhs) XR_NOEXCEPT
    {
        m_ptr = eastl::move(rhs.m_ptr);
        rhs.m_ptr = nullptr;

        m_size = eastl::move(rhs.m_size);
        rhs.m_size = 0;

        m_alloc_size = eastl::move(rhs.m_alloc_size);
        rhs.m_alloc_size = 0;

        m_allocator = eastl::move(rhs.m_allocator);

        return *this;
    }

    /// equality operator
    bool operator==(const blob& rhs) const
    {
        return (binary_compare(rhs) == 0);
    }

    /// inequality operator
    bool operator!=(const blob& rhs) const
    {
        return (binary_compare(rhs) != 0);
    }

    /// greater operator
    bool operator>(const blob& rhs) const
    {
        return (binary_compare(rhs) > 0);
    }

    /// less operator
    bool operator<(const blob& rhs) const
    {
        return (binary_compare(rhs) < 0);
    }

    /// greater-equal operator
    bool operator>=(const blob& rhs) const
    {
        return (binary_compare(rhs) >= 0);
    }

    /// less-equal operator
    bool operator<=(const blob& rhs) const
    {
        return (binary_compare(rhs) <= 0);
    }

    /// return true if the blob contains data
    XR_CONSTEXPR_CPP14_OR_INLINE bool is_valid() const
    {
        auto const result = get_ptr_unsafe();
        return result != nullptr;
    }

    /// reserve N bytes
    void reserve(size_t const size)
    {
        auto const alloc_size = get_alloc_size_unsafe();

        if(alloc_size < size)
        {
            self_delete();
            self_allocate(size);
        }

        set_size_unsafe(size);
    }

    /// trim the size member (without re-allocating!)
    void trim(size_t const size)
    {
        set_size_unsafe(size);
    }

    /// set blob contents
    void set(buffer_ref& ref)
    {
        self_copy(ref.as_pointer<void*>(), ref.length());
    }

    /// get blob size
    XR_CONSTEXPR_CPP14_OR_INLINE buffer_ref get() const
    {
        return buffer_ref { m_ptr, m_size };
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

    XR_CONSTEXPR_CPP14_OR_INLINE void set_size_unsafe(size_t const sz)
    {
        m_size = sz;
    }

    XR_CONSTEXPR_CPP14_OR_INLINE void set_size_and_ptr_unsafe(void* ptr, size_t const sz)
    {
        m_size = sz;
        m_ptr = ptr;
    }

    XR_CONSTEXPR_CPP14_OR_INLINE void set_size_and_ptr_unsafe(void* ptr, size_t const sz, size_t const alloc)
    {
        m_size = sz;
        m_ptr = ptr;
        m_alloc_size = alloc;
    }

    XR_CONSTEXPR_CPP14_OR_INLINE void* get_ptr_unsafe() const
    {
        auto const result = m_ptr;
        return result;
    }

    XR_CONSTEXPR_CPP14_OR_INLINE size_t get_size_unsafe() const
    {
        auto const result = m_size;
        return result;
    }

    XR_CONSTEXPR_CPP14_OR_INLINE size_t get_alloc_size_unsafe() const
    {
        auto const result = m_alloc_size;
        return result;
    }

    void* m_ptr;
    size_t m_size;
    size_t m_alloc_size;
    base_allocator* m_allocator;
}; // class blob

XR_NAMESPACE_END(xr, memory)
//-----------------------------------------------------------------------------------------------------------