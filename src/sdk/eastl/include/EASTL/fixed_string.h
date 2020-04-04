/////////////////////////////////////////////////////////////////////////////
// Copyright (c) Electronic Arts Inc. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// This file implements a string which uses a fixed size memory pool.
// The bEnableOverflow template parameter allows the container to resort to
// heap allocations if the memory pool is exhausted.
///////////////////////////////////////////////////////////////////////////////


#ifndef EASTL_FIXED_STRING_H
#define EASTL_FIXED_STRING_H

#include <EASTL/internal/config.h>
#include <EASTL/string.h>
#include <EASTL/internal/fixed_pool.h>

#if defined(EA_PRAGMA_ONCE_SUPPORTED)
	#pragma once // Some compilers (e.g. VC++) benefit significantly from using this. We've measured 3-4% build speed improvements in apps as a result.
#endif


namespace eastl
{
/// EASTL_FIXED_STRING_DEFAULT_NAME
///
/// Defines a default container name in the absence of a user-provided name.
/// In the case of fixed-size containers, the allocator name always refers
/// to overflow allocations.
///
#ifndef EASTL_FIXED_STRING_DEFAULT_NAME
#define EASTL_FIXED_STRING_DEFAULT_NAME EASTL_DEFAULT_NAME_PREFIX " fixed_string" // Unless the user overrides something, this is "EASTL fixed_string".
#endif



/// fixed_string
///
/// A fixed_string with bEnableOverflow == true is identical to a regular
/// string in terms of its behavior. All the expectations of regular string
/// apply to it and no additional expectations come from it. When bEnableOverflow
/// is false, fixed_string behaves like regular string with the exception that
/// its capacity can never increase. All operations you do on such a fixed_string
/// which require a capacity increase will result in undefined behavior or an
/// C++ allocation exception, depending on the configuration of EASTL.
///
/// Note: The nodeCount value is the amount of characters to allocate, which needs to
/// take into account a terminating zero. Thus if you want to store strings with a strlen
/// of 30, the nodeCount value must be at least 31.
///
/// Template parameters:
///     T                      The type of object the string holds (char, wchar_t, char8_t, char16_t, char32_t).
///     nodeCount              The max number of objects to contain.
///
/// Notes:
///     The nodeCount value must be at least 2, one for a character and one for a terminating 0.
///
///     As of this writing, the string class necessarily reallocates when an insert of
///     self is done into self. As a result, the fixed_string class doesn't support
///     inserting self into self unless the bEnableOverflow template parameter is true.
///
/// Example usage:
///    fixed_string<char, 128 + 1, true> fixedString("hello world"); // Can hold up to a strlen of 128.
///
///    fixedString = "hola mundo";
///    fixedString.clear();
///    fixedString.resize(200);
///    fixedString.sprintf("%f", 1.5f);
///
template <typename T, int nodeCount>
class fixed_string : public basic_string<T, fixed_vector_allocator<sizeof(T), nodeCount, EASTL_ALIGN_OF(T), 0> >
{
public:
    typedef fixed_vector_allocator<sizeof(T), nodeCount, EASTL_ALIGN_OF(T), 0> fixed_allocator_type;
    typedef basic_string<T, fixed_allocator_type> base_type;
    typedef fixed_string<T, nodeCount> this_type;
    typedef typename base_type::size_type size_type;
    typedef typename base_type::value_type value_type;
    typedef typename base_type::CtorDoNotInitialize CtorDoNotInitialize;
    typedef typename base_type::CtorSprintf CtorSprintf;
    typedef aligned_buffer<nodeCount * sizeof(T), EASTL_ALIGN_OF(T)> aligned_buffer_type;

    enum
    {
        kMaxSize = nodeCount - 1
    }; // -1 because we need to save one element for the silent terminating null.

    using base_type::npos;
    using base_type::mPair;
    using base_type::append;
    using base_type::resize;
    using base_type::clear;
    using base_type::capacity;
    using base_type::size;
    using base_type::sprintf_va_list;
    using base_type::DoAllocate;
    using base_type::DoFree;
    using base_type::internalLayout;
    using base_type::get_allocator;

protected:
    union // We define a union in order to avoid strict pointer aliasing issues with compilers like GCC.
    {
        value_type mArray[1];
        aligned_buffer_type mBuffer; // Question: Why are we doing this aligned_buffer thing? Why not just do an array of value_type, given that we are using just strings of char types.
    };

public:
    fixed_string();
    fixed_string(const base_type& x, size_type position, size_type n = base_type::npos); // Currently we don't support overflowAllocator specification for other constructors, for simplicity.
    fixed_string(const value_type* p, size_type n);
    fixed_string(const value_type* p);
    fixed_string(size_type n, const value_type& value);
    fixed_string(const this_type& x);
    fixed_string(const base_type& x);
    fixed_string(const value_type* pBegin, const value_type* pEnd);
    fixed_string(CtorDoNotInitialize, size_type n);
    fixed_string(CtorSprintf, const value_type* pFormat, ...);
    fixed_string(std::initializer_list<T> ilist);
    fixed_string(this_type&& x);

    this_type& operator=(const this_type& x);
    this_type& operator=(const base_type& x);
    this_type& operator=(const value_type* p);
    this_type& operator=(const value_type c);
    this_type& operator=(std::initializer_list<T> ilist);
    this_type& operator=(this_type&& x);

    void swap(this_type& x);

    void set_capacity(size_type n);
    void reset_lose_memory(); // This is a unilateral reset to an initially empty state. No destructors are called, no deallocation occurs.
    size_type max_size() const;
    bool full() const; // Returns true if the fixed space has been fully allocated. Note that if overflow is enabled, the container size can be greater than nodeCount but full() could return true because the fixed space may have a recently freed slot.
    bool has_overflowed() const; // Returns true if the allocations spilled over into the overflow allocator. Meaningful only if overflow is enabled.

    // The inherited versions of substr/left/right call the basic_string constructor,
    // which will call the overflow allocator and fail if bEnableOverflow == false
    this_type substr(size_type position, size_type n) const;
    this_type left(size_type n) const;
    this_type right(size_type n) const;
}; // fixed_string





///////////////////////////////////////////////////////////////////////
// fixed_string
///////////////////////////////////////////////////////////////////////

template <typename T, int nodeCount>
inline fixed_string<T, nodeCount>::fixed_string()
    : base_type(fixed_allocator_type(mBuffer.buffer))
{
#if EASTL_NAME_ENABLED
    get_allocator().set_name(EASTL_FIXED_STRING_DEFAULT_NAME);
#endif

    internalLayout().SetHeapBeginPtr(mArray);
    internalLayout().SetHeapCapacity(nodeCount - 1);
    internalLayout().SetHeapSize(0);

    *internalLayout().HeapBeginPtr() = 0;
}


template <typename T, int nodeCount>
inline fixed_string<T, nodeCount>::fixed_string(const this_type& x)
    : base_type(fixed_allocator_type(mBuffer.buffer))
{
#if EASTL_NAME_ENABLED
    get_allocator().set_name(x.get_allocator().get_name());
#endif

    internalLayout().SetHeapBeginPtr(mArray);
    internalLayout().SetHeapCapacity(nodeCount - 1);
    internalLayout().SetHeapSize(0);

    *internalLayout().HeapBeginPtr() = 0;

    append(x);
}


template <typename T, int nodeCount>
inline fixed_string<T, nodeCount>::fixed_string(const base_type& x)
    : base_type(fixed_allocator_type(mBuffer.buffer))
{
#if EASTL_NAME_ENABLED
    get_allocator().set_name(x.get_allocator().get_name());
#endif

    internalLayout().SetHeapBeginPtr(mArray);
    internalLayout().SetHeapCapacity(nodeCount - 1);
    internalLayout().SetHeapSize(0);

    *internalLayout().HeapBeginPtr() = 0;

    append(x);
}


template <typename T, int nodeCount>
inline fixed_string<T, nodeCount>::fixed_string(const base_type& x, size_type position, size_type n)
    : base_type(fixed_allocator_type(mBuffer.buffer))
{
#if EASTL_NAME_ENABLED
    get_allocator().set_name(x.get_allocator().get_name());
#endif

    internalLayout().SetHeapBeginPtr(mArray);
    internalLayout().SetHeapCapacity(nodeCount - 1);
    internalLayout().SetHeapSize(0);

    *internalLayout().HeapBeginPtr() = 0;

    append(x, position, n);
}


template <typename T, int nodeCount>
inline fixed_string<T, nodeCount>::fixed_string(const value_type* p, size_type n)
    : base_type(fixed_allocator_type(mBuffer.buffer))
{
#if EASTL_NAME_ENABLED
    get_allocator().set_name(EASTL_FIXED_STRING_DEFAULT_NAME);
#endif

    internalLayout().SetHeapBeginPtr(mArray);
    internalLayout().SetHeapCapacity(nodeCount - 1);
    internalLayout().SetHeapSize(0);

    *internalLayout().HeapBeginPtr() = 0;

    append(p, n);
}


template <typename T, int nodeCount>
inline fixed_string<T, nodeCount>::fixed_string(const value_type* p)
    : base_type(fixed_allocator_type(mBuffer.buffer))
{
#if EASTL_NAME_ENABLED
    get_allocator().set_name(EASTL_FIXED_STRING_DEFAULT_NAME);
#endif

    internalLayout().SetHeapBeginPtr(mArray);
    internalLayout().SetHeapCapacity(nodeCount - 1);
    internalLayout().SetHeapSize(0);

    *internalLayout().HeapBeginPtr() = 0;

    append(p); // There better be enough space to hold the assigned string.
}


template <typename T, int nodeCount>
inline fixed_string<T, nodeCount>::fixed_string(size_type n, const value_type& value)
    : base_type(fixed_allocator_type(mBuffer.buffer))
{
#if EASTL_NAME_ENABLED
    get_allocator().set_name(EASTL_FIXED_STRING_DEFAULT_NAME);
#endif

    internalLayout().SetHeapBeginPtr(mArray);
    internalLayout().SetHeapCapacity(nodeCount - 1);
    internalLayout().SetHeapSize(0);

    *internalLayout().HeapBeginPtr() = 0;

    append(n, value); // There better be enough space to hold the assigned string.
}


template <typename T, int nodeCount>
inline fixed_string<T, nodeCount>::fixed_string(const value_type* pBegin, const value_type* pEnd)
    : base_type(fixed_allocator_type(mBuffer.buffer))
{
#if EASTL_NAME_ENABLED
    get_allocator().set_name(EASTL_FIXED_STRING_DEFAULT_NAME);
#endif

    internalLayout().SetHeapBeginPtr(mArray);
    internalLayout().SetHeapCapacity(nodeCount - 1);
    internalLayout().SetHeapSize(0);

    *internalLayout().HeapBeginPtr() = 0;

    append(pBegin, pEnd);
}


template <typename T, int nodeCount>
inline fixed_string<T, nodeCount>::fixed_string(CtorDoNotInitialize, size_type n)
    : base_type(fixed_allocator_type(mBuffer.buffer))
{
#if EASTL_NAME_ENABLED
    get_allocator().set_name(EASTL_FIXED_STRING_DEFAULT_NAME);
#endif

    internalLayout().SetHeapBeginPtr(mArray);
    internalLayout().SetHeapCapacity(nodeCount - 1);

    if(n < nodeCount)
    {
        internalLayout().SetHeapSize(n);
        *internalLayout().HeapEndPtr() = 0;
    }
    else
    {
        internalLayout().SetHeapSize(0);
        *internalLayout().HeapEndPtr() = 0;

        resize(n);
    }
}


template <typename T, int nodeCount>
inline fixed_string<T, nodeCount>::fixed_string(CtorSprintf, const value_type* pFormat, ...)
    : base_type(fixed_allocator_type(mBuffer.buffer))
{
#if EASTL_NAME_ENABLED
    get_allocator().set_name(EASTL_FIXED_STRING_DEFAULT_NAME);
#endif

    internalLayout().SetHeapBeginPtr(mArray);
    internalLayout().SetHeapCapacity(nodeCount - 1);
    internalLayout().SetHeapSize(0);
    *internalLayout().HeapBeginPtr() = 0;

    va_list arguments;
    va_start(arguments, pFormat);
    sprintf_va_list(pFormat, arguments);
    va_end(arguments);
}


template <typename T, int nodeCount>
inline fixed_string<T, nodeCount>::fixed_string(this_type&& x)
    : base_type(fixed_allocator_type(mBuffer.buffer))
{
    // We copy from x instead of trade with it. We need to do so because fixed_ containers use local memory buffers.
#if EASTL_NAME_ENABLED
    get_allocator().set_name(x.get_allocator().get_name());
#endif

    internalLayout().SetHeapBeginPtr(mArray);
    internalLayout().SetHeapCapacity(nodeCount - 1);
    internalLayout().SetHeapSize(0);

    *internalLayout().HeapBeginPtr() = 0;

    append(x); // Let x destruct its own items.
}


template <typename T, int nodeCount>
inline typename fixed_string<T, nodeCount>::this_type&
fixed_string<T, nodeCount>::operator=(const this_type& x)
{
    if(this != &x)
    {
        clear();

#if EASTL_ALLOCATOR_COPY_ENABLED
        get_allocator() = x.get_allocator();
#endif

        append(x);
    }
    return *this;
}


template <typename T, int nodeCount>
inline typename fixed_string<T, nodeCount>::
this_type& fixed_string<T, nodeCount>::operator=(const base_type& x)
{
    if(static_cast<base_type*>(this) != &x)
    {
        clear();

#if EASTL_ALLOCATOR_COPY_ENABLED
        get_allocator() = x.get_allocator();
#endif

        append(x);
    }
    return *this;
}


template <typename T, int nodeCount>
inline typename fixed_string<T, nodeCount>::
this_type& fixed_string<T, nodeCount>::operator=(const value_type* p)
{
    if(internalLayout().HeapBeginPtr() != p)
    {
        clear();
        append(p);
    }
    return *this;
}


template <typename T, int nodeCount>
inline typename fixed_string<T, nodeCount>::
this_type& fixed_string<T, nodeCount>::operator=(const value_type c)
{
    clear();
    append((size_type)1, c);
    return *this;
}


template <typename T, int nodeCount>
inline typename fixed_string<T, nodeCount>::
this_type& fixed_string<T, nodeCount>::operator=(std::initializer_list<T> ilist)
{
    clear();
    append(ilist.begin(), ilist.end());
    return *this;
}


template <typename T, int nodeCount>
inline typename fixed_string<T, nodeCount>::
this_type& fixed_string<T, nodeCount>::operator=(this_type&& x)
{
    // We copy from x instead of trade with it. We need to do so because fixed_ containers use local memory buffers.

    // if(static_cast<base_type*>(this) != &x) This should be impossible, so we disable it until proven otherwise.
    {
        clear();

#if EASTL_ALLOCATOR_COPY_ENABLED
        get_allocator() = x.get_allocator();
#endif

        append(x); // Let x destruct its own items.
    }
    return *this;
}


template <typename T, int nodeCount>
inline void fixed_string<T, nodeCount>::swap(this_type& x)
{
    // Fixed containers use a special swap that can deal with excessively large buffers.
    eastl::fixed_swap(*this, x);
}


template <typename T, int nodeCount>
inline void fixed_string<T, nodeCount>::set_capacity(size_type n)
{
    const size_type nPrevSize = internalLayout().GetSize();
    const size_type nPrevCapacity = capacity();

    if(n == npos)       // If the user means to set the capacity so that it equals the size (i.e. free excess capacity)...
        n = nPrevSize;

    if(n != nPrevCapacity)  // If the request results in a capacity change...
    {
        const size_type allocSize = (n + 1); // +1 because the terminating 0 isn't included in the supplied capacity value. So now n refers the amount of memory we need.

        if(can_overflow() && (((uintptr_t)internalLayout().HeapBeginPtr() != (uintptr_t)mBuffer.buffer) || (allocSize > kMaxSize))) // If we are or would be using dynamically allocated memory instead of our fixed-size member buffer...
        {
            T* const pNewData = (allocSize <= kMaxSize) ? (T*)&mBuffer.buffer[0] : DoAllocate(allocSize);
            T* const pCopyEnd = (n < nPrevSize) ? (internalLayout().HeapBeginPtr() + n) : internalLayout().HeapEndPtr();
            CharStringUninitializedCopy(internalLayout().HeapBeginPtr(), pCopyEnd, pNewData);  // Copy [internalLayout().heap.mpBegin, pCopyEnd) to pNewData.
            if((uintptr_t)internalLayout().HeapBeginPtr() != (uintptr_t)mBuffer.buffer)
                DoFree(internalLayout().HeapBeginPtr(), internalLayout().GetHeapCapacity() + 1);

            internalLayout().SetHeapSize((size_type)(pCopyEnd - internalLayout().HeapBeginPtr()));
            internalLayout().SetHeapBeginPtr(pNewData);
            internalLayout().SetHeapCapacity(allocSize - 1);
        } // Else the new capacity would be within our fixed buffer.
        else if(n < nPrevSize) // If the newly requested capacity is less than our size, we do what vector::set_capacity does and resize, even though we actually aren't reducing the capacity.
            resize(n);
    }
}


template <typename T, int nodeCount>
inline void fixed_string<T, nodeCount>::reset_lose_memory()
{
    internalLayout().SetHeapBeginPtr(mArray);
    internalLayout().SetHeapSize(0);
    internalLayout().SetHeapCapacity(nodeCount - 1);
}


template <typename T, int nodeCount>
inline typename fixed_string<T, nodeCount>::
size_type fixed_string<T, nodeCount>::max_size() const
{
    return kMaxSize;
}


template <typename T, int nodeCount>
inline bool fixed_string<T, nodeCount>::full() const
{
    // If size >= capacity, then we are definitely full.
    // Also, if our size is smaller but we've switched away from mBuffer due to a previous overflow, then we are considered full.
    return ((size_t)(internalLayout().HeapEndPtr() - internalLayout().HeapBeginPtr()) >= kMaxSize) || ((void*)internalLayout().HeapBeginPtr() != (void*)mBuffer.buffer);
}


template <typename T, int nodeCount>
inline bool fixed_string<T, nodeCount>::has_overflowed() const
{
    // This will be incorrect for the case that bOverflowEnabled is true and the container was resized
    // down to a small size where the fixed buffer could take over ownership of the data again.
    // The only simple fix for this is to take on another member variable which tracks whether this overflow
    // has occurred at some point in the past.
    return ((void*)internalLayout().HeapBeginPtr() != (void*)mBuffer.buffer);
}


template <typename T, int nodeCount>
inline typename fixed_string<T, nodeCount>::
this_type fixed_string<T, nodeCount>::substr(size_type position, size_type n) const
{
#if EASTL_STRING_OPT_RANGE_ERRORS
    if(position > internalLayout().GetSize())
        base_type::ThrowRangeException();
#endif

    return fixed_string(internalLayout().HeapBeginPtr() + position,
        internalLayout().HeapBeginPtr() + position + eastl::min_alt(n, internalLayout().GetSize() - position));
}


template <typename T, int nodeCount>
inline typename fixed_string<T, nodeCount>::
this_type fixed_string<T, nodeCount>::left(size_type n) const
{
    const size_type nLength = size();
    if(n < nLength)
        return fixed_string(internalLayout().HeapBeginPtr(), internalLayout().HeapBeginPtr() + n);
    return *this;
}


template <typename T, int nodeCount>
inline typename fixed_string<T, nodeCount>::
this_type fixed_string<T, nodeCount>::right(size_type n) const
{
    const size_type nLength = size();
    if(n < nLength)
        return fixed_string(internalLayout().HeapEndPtr() - n, internalLayout().HeapEndPtr());
    return *this;
}


///////////////////////////////////////////////////////////////////////
// global operators
///////////////////////////////////////////////////////////////////////


// Operator +
template <typename T, int nodeCount>
fixed_string<T, nodeCount> operator+(
    const fixed_string<T, nodeCount>& a,
    const fixed_string<T, nodeCount>& b)
{
    // We have a problem here because need to return an fixed_string by value. This will typically result in it
    // using stack space equal to its size. That size may be too large to be workable.
    typedef fixed_string<T, nodeCount> this_type;

    this_type result(const_cast<this_type&>(a));
    result.append(a);
    result.append(b);
    return result;
}


template <typename T, int nodeCount>
fixed_string<T, nodeCount> operator+(
    const typename fixed_string<T, nodeCount>::value_type* p,
    const fixed_string<T, nodeCount>& b)
{
    typedef fixed_string<T, nodeCount> this_type;

    const typename this_type::size_type n = (typename this_type::size_type)CharStrlen(p);
    this_type result(const_cast<this_type&>(b).get_overflow_allocator());
    result.append(p, p + n);
    result.append(b);
    return result;
}


template <typename T, int nodeCount>
fixed_string<T, nodeCount> operator+(
    typename fixed_string<T, nodeCount>::value_type c,
    const fixed_string<T, nodeCount>& b)
{
    typedef fixed_string<T, nodeCount> this_type;

    this_type result(const_cast<this_type&>(b));
    result.push_back(c);
    result.append(b);
    return result;
}


template <typename T, int nodeCount>
fixed_string<T, nodeCount> operator+(
    const fixed_string<T, nodeCount>& a,
    const typename fixed_string<T, nodeCount>::value_type* p)
{
    typedef fixed_string<T, nodeCount> this_type;

    const typename this_type::size_type n = (typename this_type::size_type)CharStrlen(p);
    this_type result(const_cast<this_type&>(a));
    result.append(a);
    result.append(p, p + n);
    return result;
}


template <typename T, int nodeCount>
fixed_string<T, nodeCount> operator+(const fixed_string<T, nodeCount>& a, typename fixed_string<T, nodeCount>::value_type c)
{
    typedef fixed_string<T, nodeCount> this_type;

    this_type result(const_cast<this_type&>(a));
    result.append(a);
    result.push_back(c);
    return result;
}


template <typename T, int nodeCount>
fixed_string<T, nodeCount> operator+(fixed_string<T, nodeCount>&& a, fixed_string<T, nodeCount>&& b)
{
    a.append(b); // Using an rvalue by name results in it becoming an lvalue.
    return eastl::move(a);
}

template <typename T, int nodeCount>
fixed_string<T, nodeCount> operator+(fixed_string<T, nodeCount>&& a, const fixed_string<T, nodeCount>& b)
{
    a.append(b);
    return eastl::move(a);
}

template <typename T, int nodeCount>
fixed_string<T, nodeCount> operator+(
    const typename fixed_string<T, nodeCount>::value_type* p,
    fixed_string<T, nodeCount>&& b)
{
    b.insert(0, p);
    return eastl::move(b);
}

template <typename T, int nodeCount>
fixed_string<T, nodeCount> operator+(
    fixed_string<T, nodeCount>&& a,
    const typename fixed_string<T, nodeCount>::value_type* p)
{
    a.append(p);
    return eastl::move(a);
}

template <typename T, int nodeCount>
fixed_string<T, nodeCount> operator+(
    fixed_string<T, nodeCount>&& a,
    typename fixed_string<T, nodeCount>::value_type c)
{
    a.push_back(c);
    return eastl::move(a);
}


// operator ==, !=, <, >, <=, >= come from the string implementations.

template <typename T, int nodeCount>
inline void swap(fixed_string<T, nodeCount>& a,
    fixed_string<T, nodeCount>& b)
{
    // Fixed containers use a special swap that can deal with excessively large buffers.
    eastl::fixed_swap(a, b);
}


} // namespace eastl

#endif // Header include guard
