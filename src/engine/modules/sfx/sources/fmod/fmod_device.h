// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/memory/memory_allocator_base.h"

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, sfx, fmod_impl)

//-----------------------------------------------------------------------------------------------------------
class fmod_device
{
public:
    fmod_device(memory::base_allocator& alloc);
    XR_DECLARE_DEFAULT_DESTRUCTOR(fmod_device);
    XR_DECLARE_DELETE_COPY_ASSIGNMENT(fmod_device);
    XR_DECLARE_DELETE_MOVE_ASSIGNMENT(fmod_device);

    void initialize();
    void destroy();

    FMOD::Studio::System* get_fmod_system();
    memory::base_allocator& get_allocator();

private:
    static void* fmod_malloc();
    static void fmod_free();

    memory::base_allocator& m_allocator;
    FMOD::Studio::System* m_system;
}; // class fmod_device

//-----------------------------------------------------------------------------------------------------------
/**
 */
inline FMOD::Studio::System* fmod_device::get_fmod_system()
{
    return m_system;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
inline memory::base_allocator& fmod_device::get_allocator()
{
    return m_allocator;
}

XR_NAMESPACE_END(xr, sfx, fmod_impl)
//-----------------------------------------------------------------------------------------------------------