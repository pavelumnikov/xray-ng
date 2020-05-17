// This file is a part of xray-ng engine
//

#include "config.h"
#include "corlib/utils/unordered_map.h"
#include "corlib/utils/fixed_string.h"
#include "corlib/memory/uninitialized_reference.h"
#include "corlib/memory/proxy/eastl_proxy_allocator.h"
#include <stdio.h>
#include "constants.h"

//-----------------------------------------------------------------------------------------------------------
namespace xr
{

using config_key = utils::fixed_string64;
using config_value = utils::fixed_string128;
using config_map = utils::unordered_map<config_key, config_value>;
static memory::uninitialized_reference<config_map> the_config_map;

//-----------------------------------------------------------------------------------------------------------
namespace details
{

//-----------------------------------------------------------------------------------------------------------
/**
 */
void parse_line_and_append(utils::string_view view, config_map& m)
{
    utils::string_view line_delims { " " };

    const auto first = view.cbegin();
    auto second = eastl::find_first_of(view.cbegin(), view.cend(), line_delims.cbegin(),line_delims.cend());

    config_key k { first, size_t(second - first) };

    second = eastl::next(second);
    config_value v { second, size_t(view.cend() - second) };

    auto pair = eastl::make_pair(eastl::move(k), eastl::move(v));
    m.insert(eastl::move(pair));
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
void close_config_file(FILE* f)
{
    if(f != nullptr)
        fclose(f);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
void save_config_to_file(FILE* file, config_map const& map)
{
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
void load_config_from_file(FILE* file, config_map& map)
{
    int size = 512, pos, c;
    char temp_buffer[512] { 0 };
    do
    { // read all lines in file
        pos = 0;
        do
        { // read one line
            c = fgetc(file);
            if(c != EOF) temp_buffer[pos++] = (char)c;
            if(pos >= size - 1)
            { // skip for now
                continue;
            }
        } while(c != EOF && c != '\n');
        temp_buffer[pos] = 0;
        if(strlen(temp_buffer) > 0)
        {
            // line is now in buffer
            parse_line_and_append(temp_buffer, map);
        }
    } while(c != EOF);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
bool check_key_exists(config_map& map, config_key const& key)
{
    return map.find(key) != map.end();
}

} // namespace details
//-----------------------------------------------------------------------------------------------------------

#define XR_CONFIG_CHECK_CONSTRUCTED \
    XR_DEBUG_ASSERTION_MSG(the_config_map.is_constructed(), "config must be initialized")

//-----------------------------------------------------------------------------------------------------------
/**
 */
void initialize_config(memory::base_allocator& alloc)
{
    XR_DEBUG_ASSERTION_MSG(!the_config_map.is_constructed(), "config already initialized");
    memory::construct_reference(the_config_map, memory::proxy::eastl_proxy_allocator { alloc });
    load_config();
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
void shutdown_config()
{
    XR_CONFIG_CHECK_CONSTRUCTED;
    memory::destruct_reference(the_config_map);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
void load_config()
{
    XR_CONFIG_CHECK_CONSTRUCTED;
    FILE* file = fopen(constants::xrayng_default_config, "a+");
    details::load_config_from_file(file, the_config_map.ref());
    fclose(file);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
void save_config()
{
    XR_CONFIG_CHECK_CONSTRUCTED;
    FILE* file = fopen(constants::xrayng_default_config, "w+");
    details::save_config_to_file(file, the_config_map.ref());
    fclose(file);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
bool find_config_value(utils::string_view key, const char** value)
{
    XR_CONFIG_CHECK_CONSTRUCTED;
    config_map& m = the_config_map.ref();
    config_key k { key.data() };

    if(!details::check_key_exists(m, k))
        return false;

    *value = m[k].data();
    return true;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
void set_config_value(utils::string_view key, utils::string_view value)
{
    XR_CONFIG_CHECK_CONSTRUCTED;
    config_map& m = the_config_map.ref();
    config_key k { key.data() };

    if(!details::check_key_exists(m, k))
    {
        config_value v { value.data() };
        auto pair = eastl::make_pair(eastl::move(k), eastl::move(v));
        m.insert(eastl::move(pair));
    }
    else
    {
        m[k].assign(value.cbegin(), value.cend());
    }
}

} // namespace xr
//-----------------------------------------------------------------------------------------------------------
