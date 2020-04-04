#include "corlib/sys/arg_list.h"
#include "corlib/etl/string/constexpr_string.h"

//------------------------------------------------------------------------------
namespace xr::sys
{

//------------------------------------------------------------------------------
/**
*/
arg_list::arg_list(memory::base_allocator& alloc, eastl::string_view cmd_line)
    : m_allocator { alloc }
    , m_map { m_allocator }
{
    eastl::string_view line_delims { " " };
    eastl::string_view kv_delims { "=" };

    auto first = eastl::cbegin(cmd_line);
    while(first != eastl::cend(cmd_line))
    {
        const auto second = eastl::find_first_of(first, eastl::cend(cmd_line),
            eastl::cbegin(line_delims), eastl::cend(line_delims));

        if(first != second)
        {
            eastl::string_view key { first, size_t(second - first) };
            auto value = eastl::find_first_of(eastl::cbegin(key), second,
                eastl::cbegin(kv_delims), eastl::cend(kv_delims));

            eastl::string_view key_data { key.data(), size_t(value - first) };
            if(value != std::cend(key))
            {
                value = std::next(value);
                eastl::string_view value_data { value, size_t(second - value) };
                add_arg(key_data, value_data);
            }
            else
            {
                add_arg(key_data);
            }
        }

        if(second == eastl::cend(cmd_line))
            break;

        first = eastl::next(second);
    }
}

//------------------------------------------------------------------------------
/**
*/
bool arg_list::add_arg(eastl::string_view key) noexcept
{
    key_type const k(eastl::begin(key), eastl::end(key));

    if(internal_key_exists(k))
        return false;

    auto const result = m_map.emplace(k, value_type());
    return result.second;
}

//------------------------------------------------------------------------------
/**
*/
bool arg_list::add_arg(eastl::string_view key, eastl::string_view arg) noexcept
{
    key_type const k(eastl::begin(key), eastl::end(key));

    if(internal_key_exists(k))
        return false;

    auto const result = m_map.emplace(eastl::piecewise_construct,
        eastl::forward_as_tuple(k), 
        eastl::forward_as_tuple(eastl::begin(arg), eastl::end(arg)));

    return result.second;
}

//------------------------------------------------------------------------------
/**
*/
bool arg_list::has_arg(eastl::string_view key) const noexcept
{
    key_type const k(eastl::begin(key), eastl::end(key));
    return internal_key_exists(k);
}

//------------------------------------------------------------------------------
/**
*/
eastl::string_view arg_list::at(eastl::string_view key) const noexcept
{
    key_type const k(eastl::begin(key), eastl::end(key));

    XR_DEBUG_ASSERTION(m_map.find(k) != eastl::end(m_map));
    return etl::string::to_string_view(m_map.at(k));
}

//------------------------------------------------------------------------------
/**
*/
bool arg_list::internal_key_exists(key_type const& k) const noexcept
{
    return m_map.find(k) != eastl::end(m_map);
}

} // namespace xr::sys
//------------------------------------------------------------------------------
