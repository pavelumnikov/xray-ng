/////////////////////////////////////////////////////////////////////////////
// Copyright (c) Electronic Arts Inc. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#ifndef EASTL_ALLOCATOR_H
#define EASTL_ALLOCATOR_H


#include <EASTL/internal/config.h>
#include <EASTL/EABase/nullptr.h>
#include <stddef.h>


#ifdef _MSC_VER
	#pragma warning(push)
	#pragma warning(disable: 4189) // local variable is initialized but not referenced
#endif

#if defined(EA_PRAGMA_ONCE_SUPPORTED)
	#pragma once // Some compilers (e.g. VC++) benefit significantly from using this. We've measured 3-4% build speed improvements in apps as a result.
#endif



namespace eastl
{
/// dummy_allocator
///
/// Defines an allocator which does nothing. It returns NULL from allocate calls.
///
class EASTL_API dummy_allocator
{
public:
	EASTL_ALLOCATOR_EXPLICIT dummy_allocator(const char* = NULL)
	{}
	dummy_allocator(const dummy_allocator&)
	{}
	dummy_allocator(const dummy_allocator&, const char*)
	{}

	dummy_allocator& operator=(const dummy_allocator&)
	{
		return *this;
	}

	void* allocate(size_t, int = 0)
	{
		return NULL;
	}
	void* allocate(size_t, size_t, size_t, int = 0)
	{
		return NULL;
	}
	void  deallocate(void*, size_t)
	{}

	const char* get_name() const
	{
		return "";
	}
	void        set_name(const char*)
	{}
};

inline bool operator==(const dummy_allocator&, const dummy_allocator&)
{
	return true;
}
inline bool operator!=(const dummy_allocator&, const dummy_allocator&)
{
	return false;
}

/// allocate_memory
///
/// This is a memory allocation dispatching function.
/// To do: Make aligned and unaligned specializations.
///        Note that to do this we will need to use a class with a static
///        function instead of a standalone function like below.
///
template <typename Allocator>
void* allocate_memory(Allocator& a, size_t n, size_t alignment, size_t alignmentOffset);


} // namespace eastl

namespace eastl
{
static dummy_allocator default_dummy_allocator {};

/// allocate_memory
///
/// This is a memory allocation dispatching function.
/// To do: Make aligned and unaligned specializations.
///        Note that to do this we will need to use a class with a static
///        function instead of a standalone function like below.
///
template <typename Allocator>
inline void* allocate_memory(Allocator& a, size_t n, size_t alignment, size_t alignmentOffset)
{
	void* result;
	if(alignment <= EASTL_ALLOCATOR_MIN_ALIGNMENT)
	{
		result = EASTLAlloc(a, n);
		// Ensure the result is correctly aligned.  An assertion likely indicates a mismatch between EASTL_ALLOCATOR_MIN_ALIGNMENT and the minimum alignment
		// of EASTLAlloc.  If there is a mismatch it may be necessary to define EASTL_ALLOCATOR_MIN_ALIGNMENT to be the minimum alignment of EASTLAlloc, or
		// to increase the alignment of EASTLAlloc to match EASTL_ALLOCATOR_MIN_ALIGNMENT.
		EASTL_ASSERT((reinterpret_cast<size_t>(result) & ~(alignment - 1)) == reinterpret_cast<size_t>(result));
	}
	else
	{
		result = EASTLAllocAligned(a, n, alignment, alignmentOffset);
		// Ensure the result is correctly aligned.  An assertion here may indicate a bug in the allocator.
		EASTL_ASSERT((reinterpret_cast<size_t>(result) & ~(alignment - 1)) == reinterpret_cast<size_t>(result));
	}
	return result;
}

}

#ifdef _MSC_VER
	#pragma warning(pop)
#endif


#endif // Header include guard
















