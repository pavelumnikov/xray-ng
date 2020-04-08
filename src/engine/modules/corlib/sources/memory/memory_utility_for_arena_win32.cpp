// This file is a part of xray-ng engine
//

#pragma once

#if !defined(XRAY_PLATFORM_WINDOWS)
#   error "This code is supported by Windows platform!"
#endif // !defined(XRAY_PLATFORM_WINDOWS)

#include "memory_utility_for_arena_win32.h"
#include <cassert>
#include <new.h>
#include <malloc.h>

//------------------------------------------------------------------------------
namespace xr::memory
{

//------------------------------------------------------------------------------
namespace
{

//------------------------------------------------------------------------------
/**
*/
static int heap_walk(
    HANDLE heap_handle,
    struct _heapinfo* _entry
)
{
    PROCESS_HEAP_ENTRY Entry;
    DWORD errval;
    int errflag;
    int retval = _HEAPOK;

    Entry.wFlags = 0;
    Entry.iRegionIndex = 0;
    Entry.cbData = 0;
    if((Entry.lpData = _entry->_pentry) == NULL)
    {
        if(!HeapWalk(heap_handle, &Entry))
        {
            if(GetLastError() == ERROR_CALL_NOT_IMPLEMENTED)
            {
                _doserrno = ERROR_CALL_NOT_IMPLEMENTED;
                errno = ENOSYS;
                return _HEAPEND;
            }
            return _HEAPBADBEGIN;
        }
    }
    else
    {
        if(_entry->_useflag == _USEDENTRY)
        {
            if(!HeapValidate(heap_handle, 0, _entry->_pentry))
                return _HEAPBADNODE;
            Entry.wFlags = PROCESS_HEAP_ENTRY_BUSY;
        }
nextBlock:
        /*
         * Guard the HeapWalk call in case we were passed a bad pointer
         * to an allegedly free block.
         */
        __try
        {
            errflag = 0;
            if(!HeapWalk(heap_handle, &Entry))
                errflag = 1;
        }
        __except(EXCEPTION_EXECUTE_HANDLER)
        {
            errflag = 2;
        }

        /*
         * Check errflag to see how HeapWalk fared...
         */
        if(errflag == 1)
        {
            /*
             * HeapWalk returned an error.
             */
            if((errval = GetLastError()) == ERROR_NO_MORE_ITEMS)
            {
                return _HEAPEND;
            }
            else if(errval == ERROR_CALL_NOT_IMPLEMENTED)
            {
                _doserrno = errval;
                errno = ENOSYS;
                return _HEAPEND;
            }
            return _HEAPBADNODE;
        }
        else if(errflag == 2)
        {
            /*
             * Exception occurred during the HeapWalk!
             */
            return _HEAPBADNODE;
        }
    }

    if(Entry.wFlags & (PROCESS_HEAP_REGION |
        PROCESS_HEAP_UNCOMMITTED_RANGE))
    {
        goto nextBlock;
    }

    _entry->_pentry = (int*)Entry.lpData;
    _entry->_size = Entry.cbData;
    if(Entry.wFlags & PROCESS_HEAP_ENTRY_BUSY)
    {
        _entry->_useflag = _USEDENTRY;
    }
    else
    {
        _entry->_useflag = _FREEENTRY;
    }

    return(retval);
}

} // anonymous namespace

//------------------------------------------------------------------------------
/**
*/
size_t
mem_usage(HANDLE heap_handle, uint32_t* used, uint32_t* freed)
{
    if(!::IsDebuggerPresent())
        return (0);

    _HEAPINFO hinfo;
    int heapstatus;
    hinfo._pentry = nullptr;
    size_t total = 0;
    uint32_t blocks_free = 0;
    uint32_t blocks_used = 0;

    while((heapstatus = heap_walk(heap_handle, &hinfo)) == _HEAPOK)
    {
        if(hinfo._useflag == _USEDENTRY)
        {
            total += hinfo._size;
            blocks_used += 1;
        }
        else
        {
            blocks_free += 1;
        }
    }

    if(freed)
    {
        *freed = 1024 * blocks_free;
    }

    if(used)
    {
        *used = 1024 * blocks_used;
    }

    switch(heapstatus)
    {
        case _HEAPEMPTY:
            break;

        case _HEAPEND:
            break;

        case _HEAPBADPTR:
            ::OutputDebugStringA("bad pointer to heap");
            return 0;

        case _HEAPBADBEGIN:
            ::OutputDebugStringA("bad start of heap");
            return 0;

        case _HEAPBADNODE:
            ::OutputDebugStringA("bad node in heap");
            return 0;
    }

    return total;
}

} // namespace xr::memory
//------------------------------------------------------------------------------