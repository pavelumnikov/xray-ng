// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/types.h"

//-----------------------------------------------------------------------------------------------------------
namespace xr::tasks
{

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


    task_group() noexcept;
    explicit task_group(predefined_values v) noexcept;
    explicit task_group(int16_t id) noexcept;

    static task_group get_default_group() noexcept;
    task_group& operator= (const predefined_values& v) noexcept;

    bool operator== (const predefined_values& v) const noexcept;
    bool operator== (const task_group& other) const noexcept;
    bool operator!= (const task_group& other) const noexcept;

    int get_valid_index() const noexcept;
    bool is_valid() const noexcept;

private:
    int16_t id;
}; // class task_group

//-----------------------------------------------------------------------------------------------------------
/**
*/
inline task_group::task_group() noexcept
    : id { invalid }
{}

//-----------------------------------------------------------------------------------------------------------
/**
 */
inline task_group::task_group(predefined_values v) noexcept
    : id { static_cast<int16_t>(v) }
{}

//-----------------------------------------------------------------------------------------------------------
/**
 */
inline task_group::task_group(int16_t id) noexcept
    : id { id }
{}

//-----------------------------------------------------------------------------------------------------------
/**
 */
inline task_group
task_group::get_default_group() noexcept
{
    return task_group(default_group);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
inline task_group&
task_group::operator= (const predefined_values& v) noexcept
{
    id = (int16_t)v;
    return *this;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
inline bool
task_group::operator== (const predefined_values& v) const noexcept
{
    return (id == v);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
inline bool
task_group::operator== (const task_group& other) const noexcept
{
    return (id == other.id);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
inline bool
task_group::operator!= (const task_group& other) const noexcept
{
    return (id != other.id);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
inline int
task_group::get_valid_index() const noexcept
{
    //MT_ASSERT(is_valid(), "Try to get invalid index");
    return id;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
inline bool
task_group::is_valid() const noexcept
{
    if(id == invalid)
        return false;

    if(id == assign_from_context)
        return false;

    return (id >= 0 && id < max_groups_count);
}

} // namespace xr::tasks
//-----------------------------------------------------------------------------------------------------------
