// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/types.h"

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, tasks)

//-----------------------------------------------------------------------------------------------------------
class task_group
{
public:
    static constexpr uint16_t max_groups_count = 256;
    enum predefined_values
    {
        default_group = 0,
        invalid = -1,
        assign_from_context = -2
    };


    task_group() XR_NOEXCEPT;
    explicit task_group(predefined_values v) XR_NOEXCEPT;
    explicit task_group(int16_t id) XR_NOEXCEPT;

    static task_group get_default_group() XR_NOEXCEPT;
    task_group& operator= (const predefined_values& v) XR_NOEXCEPT;

    bool operator== (const predefined_values& v) const XR_NOEXCEPT;
    bool operator== (const task_group& other) const XR_NOEXCEPT;
    bool operator!= (const task_group& other) const XR_NOEXCEPT;

    int get_valid_index() const XR_NOEXCEPT;
    bool is_valid() const XR_NOEXCEPT;

private:
    int16_t id;
}; // class task_group

//-----------------------------------------------------------------------------------------------------------
/**
*/
inline task_group::task_group() XR_NOEXCEPT
    : id { invalid }
{}

//-----------------------------------------------------------------------------------------------------------
/**
 */
inline task_group::task_group(predefined_values v) XR_NOEXCEPT
    : id { static_cast<int16_t>(v) }
{}

//-----------------------------------------------------------------------------------------------------------
/**
 */
inline task_group::task_group(int16_t id) XR_NOEXCEPT
    : id { id }
{}

//-----------------------------------------------------------------------------------------------------------
/**
 */
inline task_group
task_group::get_default_group() XR_NOEXCEPT
{
    return task_group(default_group);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
inline task_group&
task_group::operator= (const predefined_values& v) XR_NOEXCEPT
{
    id = (int16_t)v;
    return *this;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
inline bool
task_group::operator== (const predefined_values& v) const XR_NOEXCEPT
{
    return (id == v);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
inline bool
task_group::operator== (const task_group& other) const XR_NOEXCEPT
{
    return (id == other.id);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
inline bool
task_group::operator!= (const task_group& other) const XR_NOEXCEPT
{
    return (id != other.id);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
inline int
task_group::get_valid_index() const XR_NOEXCEPT
{
    XR_DEBUG_ASSERTION_MSG(is_valid(), "Try to get invalid index");
    return id;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
inline bool
task_group::is_valid() const XR_NOEXCEPT
{
    if(id == invalid)
        return false;

    if(id == assign_from_context)
        return false;

    return (id >= 0 && id < max_groups_count);
}

XR_NAMESPACE_END(xr, tasks)
//-----------------------------------------------------------------------------------------------------------
