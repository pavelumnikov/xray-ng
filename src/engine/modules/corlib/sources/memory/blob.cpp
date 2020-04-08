// This file is a part of xray-ng engine
//

#include "corlib/memory/blob.h"
#include "corlib/memory/memory_functions.h"
#include <cassert>
#include <cstring>

namespace xr::memory
{

void blob::self_allocate(size_t const size)
{
    assert(!this->is_valid());
    auto const this_ptr = m_allocator->malloc_impl(size
        XR_DEBUG_PARAMETERS_DESCRIPTION_DEFINITION("blob allocation")
        XR_DEBUG_PARAMETERS_DEFINITION);

    this->set_size_and_ptr_unsafe(this_ptr, size, size);
}

void blob::self_delete()
{
    if(this->is_valid())
    {
        auto const this_ptr = this->get_ptr_unsafe();

        XR_DEBUG_ASSERTION(this_ptr != nullptr);
        m_allocator->free_impl(this_ptr
            XR_DEBUG_PARAMETERS_DEFINITION);

        this->set_size_and_ptr_unsafe(nullptr, 0, 0);
    }
}

void blob::self_copy(const void* ptr, size_t const size)
{
    XR_DEBUG_ASSERTION((ptr != nullptr) && (size > 0));

    auto const alloc_size = this->get_alloc_size_unsafe();
    auto const this_ptr = this->get_ptr_unsafe();

    // only re-allocate if not enough space
    if((this_ptr == nullptr) || (alloc_size < size))
    {
        this->self_delete();
        this->self_allocate(size);
    }

    this->set_size_unsafe(size);
    copy(this_ptr, size, ptr, size);
}

int blob::binary_compare(const blob& rhs) const
{
    auto const this_size = this->get_size_unsafe();
    auto const rhs_size = rhs.get_size_unsafe();

    auto const this_ptr = this->get_ptr_unsafe();
    auto const rhs_ptr = rhs.get_ptr_unsafe();

    XR_DEBUG_ASSERTION(this_ptr != nullptr);
    XR_DEBUG_ASSERTION(rhs_ptr != nullptr);

    if(this_size == rhs_size)
    {
        return memcmp(this_ptr, rhs_ptr, this_size);
    }

    if(this_size > rhs_size)
    {
        return 1;
    }

    return -1;
}

size_t blob::hash_code() const
{
    size_t hash = 0;
    auto* ptr = const_cast<const int8_t*>(static_cast<int8_t*>(this->m_ptr));
    auto const size = this->get_size_unsafe();

    for(size_t i = 0; i < size; i++)
    {
        hash += ptr[i];
        hash += hash << 10;
        hash ^= hash >> 6;
    }

    hash += hash << 3;
    hash ^= hash >> 11;
    hash += hash << 15;

    return hash;
}

} // namespace aztek::core::memory