// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/memory/allocator_helper.h"
#include <malloc.h> // for _alloca
#include <new> // for new placement

//-----------------------------------------------------------------------------------------------------------
#define XR_ALLOCATE_MEMORY(allocator, size, desc) \
    (allocator).malloc_impl((size) \
        XR_DEBUG_PARAMETERS_DESCRIPTION_DEFINITION(desc) \
        XR_DEBUG_PARAMETERS_DEFINITION)

//-----------------------------------------------------------------------------------------------------------
#define XR_REALLOCATE_MEMORY(allocator, ptr, size, desc) \
    (allocator).realloc_impl((ptr), (size) \
        XR_DEBUG_PARAMETERS_DESCRIPTION_DEFINITION(desc) \
        XR_DEBUG_PARAMETERS_DEFINITION)

//-----------------------------------------------------------------------------------------------------------
#define XR_ALLOCATE_MEMORY_T(allocator, type, desc) \
    reinterpret_cast<type*>((allocator).malloc_impl(sizeof(type) \
        XR_DEBUG_PARAMETERS_DESCRIPTION_DEFINITION(desc) \
        XR_DEBUG_PARAMETERS_DEFINITION))

//-----------------------------------------------------------------------------------------------------------
#define XR_ALLOCATE_OBJECT_ARRAY_T(allocator, type, count, desc) \
    reinterpret_cast<type*>((allocator).malloc_impl(sizeof(type) * (count) \
        XR_DEBUG_PARAMETERS_DESCRIPTION_DEFINITION(desc) \
        XR_DEBUG_PARAMETERS_DEFINITION))

//-----------------------------------------------------------------------------------------------------------
#define XR_ALLOCATE_OBJECT_T(allocator, type, desc) \
    ::new(reinterpret_cast<xr::pvoid>(XR_ALLOCATE_MEMORY_T(allocator, type, desc))) type

//-----------------------------------------------------------------------------------------------------------
#define XR_DEALLOCATE_MEMORY(allocator, ptr) \
    (allocator).free_impl(ptr XR_DEBUG_PARAMETERS_DEFINITION)

//-----------------------------------------------------------------------------------------------------------
#define XR_DEALLOCATE_MEMORY_T(allocator, ptr) \
    xr::memory::call_destruct(ptr); \
    XR_DEALLOCATE_MEMORY(allocator, reinterpret_cast<xr::pvoid>(ptr))

//-----------------------------------------------------------------------------------------------------------
#define XR_STACK_ALLOCATE_MEMORY(size) _alloca((size))

//-----------------------------------------------------------------------------------------------------------
#define XR_STACK_ALLOCATE_ARRAY_T(type, count) (((type)*))XR_STACK_ALLOCATE_MEMORY(sizeof(type) * (count))

//-----------------------------------------------------------------------------------------------------------
#define XR_STACK_ALIGN_ALLOCATE_MEMORY(size, align) \
    reinterpret_cast<xr::pvoid>((((intptr_t)_alloca( (x)+(align-1) )) + (align-1)) & ~(align-1)))

//-----------------------------------------------------------------------------------------------------------
#define XR_STACK_ALIGN16_ALLOCATE_MEMORY(size) XR_STACK_ALIGN_ALLOCATE_MEMORY(size, 16)

//-----------------------------------------------------------------------------------------------------------
