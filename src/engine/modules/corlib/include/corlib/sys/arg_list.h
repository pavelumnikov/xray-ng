// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/etl/string/fixed_string.h"
#include "corlib/etl/containers/std/unordered_map.h"

//------------------------------------------------------------------------------
namespace xr::sys
{

//------------------------------------------------------------------------------
/** Parses command line arguments and stores them for look-up by the user.
 */
class arg_list final
{
public:
    using key_type = etl::string::fixed_string128;
    using value_type = etl::string::fixed_string128;

    XR_DECLARE_DELETE_COPY_ASSIGNMENT(arg_list);
    XR_DECLARE_DELETE_MOVE_ASSIGNMENT(arg_list);
    XR_DECLARE_DEFAULT_DESTRUCTOR(arg_list);

    /** Parses command line string
     *  \param alloc memory allocator for inner objects
     *  \param cmd_line the command line string
    */
    arg_list(memory::base_allocator& alloc, eastl::string_view cmd_line);

    /** Adds a key with no arguments to the list
        \param key
    */
    bool add_arg(eastl::string_view key) noexcept;

    /** Adds an arg to an existing key or creates an key for the arg
        \param key the key the arg is associated with
        \param arg the value
    */
    bool add_arg(eastl::string_view key, eastl::string_view arg) noexcept;

    /** Check if the key already exists within the map
        \param key the key to check for
    */
    bool has_arg(eastl::string_view key) const noexcept;

    /** Return the value associated with key, or asserts if key has no values or multiple values
        \param key the key to get the value for
    */
    eastl::string_view at(eastl::string_view key) const noexcept;
    eastl::string_view operator[](eastl::string_view key) const noexcept;

private:
    using arg_map = etl::containers::std::unordered_map<key_type, value_type>;
    using arg_map_allocator = memory::proxy::eastl_proxy_allocator;

    bool internal_key_exists(key_type const& k) const noexcept;

    arg_map_allocator m_allocator;
    arg_map m_map;
}; // class arg_list

//------------------------------------------------------------------------------
/**
*/
inline eastl::string_view
arg_list::operator[](eastl::string_view key) const noexcept
{
    return at(key);
}

} // namespace xr::sys
//------------------------------------------------------------------------------