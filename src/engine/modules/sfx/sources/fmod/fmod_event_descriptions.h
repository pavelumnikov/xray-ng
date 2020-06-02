// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/memory/memory_allocator_base.h"
#include "corlib/utils/fixed_string.h"
#include "corlib/utils/string_map.h"

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, sfx, fmod_impl)

//-----------------------------------------------------------------------------------------------------------
class fmod_event_descriptions
{
public:
    fmod_event_descriptions(memory::base_allocator& alloc, FMOD::Studio::System* system);
    XR_DECLARE_DEFAULT_DESTRUCTOR(fmod_event_descriptions);
    XR_DECLARE_DELETE_COPY_ASSIGNMENT(fmod_event_descriptions);
    XR_DECLARE_DELETE_MOVE_ASSIGNMENT(fmod_event_descriptions);

    FMOD::Studio::EventDescription* add_or_get_from_cache(utils::string_view name);
    memory::base_allocator& get_allocator();

private:
    memory::proxy::eastl_proxy_allocator m_proxy_allocator;
    FMOD::Studio::System* m_fmod_system;
    utils::string_map<FMOD::Studio::EventDescription*> m_descriptions;
}; // class fmod_event_descriptions

//-----------------------------------------------------------------------------------------------------------
/**
 */
inline memory::base_allocator& fmod_event_descriptions::get_allocator()
{
    return m_proxy_allocator.get_base_allocator();
}

XR_NAMESPACE_END(xr, sfx, fmod_impl)
//-----------------------------------------------------------------------------------------------------------