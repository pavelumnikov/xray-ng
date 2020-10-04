// This file is a part of xray-ng engine
//

#include "corlib/sys/arg_list.h"

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, sys)

//-----------------------------------------------------------------------------------------------------------
/**
*/
arg_list::arg_list(memory::base_allocator& alloc, utils::string_view cmd_line)
    : m_allocator { alloc }
    , m_map { m_allocator }
{
    utils::string_view line_delims { " " };
    utils::string_view kv_delims { "=" };

    auto first = cmd_line.cbegin();
    while(first != cmd_line.cend())
    {
        pcstr second = eastl::find_first_of(first, cmd_line.cend(),
            line_delims.cbegin(), line_delims.cend());

        if(first != second)
        {
            utils::string_view key { first, size_t(second - first) };
            pcstr value = eastl::find_first_of(key.cbegin(), second, kv_delims.cbegin(), kv_delims.cend());

            utils::string_view key_data { key.data(), size_t(value - first) };
            if(value != key.cend())
            {
                value = eastl::next(value);
                utils::string_view value_data { value, size_t(second - value) };
                add_arg(key_data, value_data);
            }
            else
            {
                add_arg(key_data);
            }
        }

        if(second == cmd_line.cend())
            break;

        first = eastl::next(second);
    }
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
bool arg_list::add_arg(utils::string_view key) XR_NOEXCEPT
{
    key_type const k(key.cbegin(), key.cend());

    if(internal_key_exists(k))
        return false;

    auto const result = m_map.emplace(k, value_type());
    return result.second;
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
bool arg_list::add_arg(utils::string_view key, utils::string_view arg) XR_NOEXCEPT
{
    key_type const k(key.cbegin(), key.cend());

    if(internal_key_exists(k))
        return false;

    auto const result = m_map.emplace(eastl::piecewise_construct,
        eastl::forward_as_tuple(k), 
        eastl::forward_as_tuple(arg.cbegin(), arg.cend()));

    return result.second;
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
bool arg_list::has_arg(utils::string_view key) const XR_NOEXCEPT
{
    key_type const k(key.cbegin(), key.cend());
    return internal_key_exists(k);
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
utils::string_view arg_list::at(utils::string_view key) const XR_NOEXCEPT
{
    key_type const k(key.cbegin(), key.cend());

    XR_DEBUG_ASSERTION(m_map.find(k) != eastl::end(m_map));
    return utils::to_string_view(m_map.at(k));
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
bool arg_list::internal_key_exists(key_type const& k) const XR_NOEXCEPT
{
    return m_map.find(k) != eastl::end(m_map);
}

XR_NAMESPACE_END(xr, sys)
//-----------------------------------------------------------------------------------------------------------
