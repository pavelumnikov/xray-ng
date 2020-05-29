// This file is a part of xray-ng engine
//

#pragma once

#include "extension/subsystem.h"
#include "corlib/utils/ptr.h"
#include "corlib/utils/static_vector.h"

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, extension)

//-----------------------------------------------------------------------------------------------------------
enum class tick_group
{
    variable,
    smoothed
}; // enum class tick_group

//-----------------------------------------------------------------------------------------------------------
struct subsystem_wrapper
{
    subsystem_wrapper(utils::shared_ptr<subsystem> const& ptr, tick_group group);
    utils::shared_ptr<subsystem> ptr;
    tick_group group;
}; // struct subsystem_wrapper

subsystem_wrapper::subsystem_wrapper(utils::shared_ptr<subsystem> const& ptr, tick_group group)
    : ptr(ptr)
    , group(group)
{}

//-----------------------------------------------------------------------------------------------------------
class context
{
public:
    context(memory::base_allocator& alloc);
    ~context();

    template<typename T, typename ... Args>
    void register_subsystem(tick_group group, Args... args);

    template<typename T>
    utils::shared_ptr<T> get_subsystem() const;

    // Initialize subsystems
    bool initialize_async(tasks::execution_context& ctx);
    // Tick
    void tick_async(tasks::execution_context& ctx, tick_group group, float delta = 0.0f);

private:
    template<typename T>
    static void validate_subsystem();

    memory::base_allocator& m_allocator;
    utils::static_vector<subsystem_wrapper, 32> m_subsystems;
}; // class context

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<typename T, typename ... Args>
inline void context::register_subsystem(tick_group group, Args... args)
{
    validate_subsystem<T>();
    auto sub = utils::make_shared_ptr<subsystem_wrapper>(m_allocator, this, args...);
    m_subsystems.emplace_back(eastl::move(sub), group);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<typename T>
inline utils::shared_ptr<T> context::get_subsystem() const
{
    validate_subsystem<T>();
    for(size_t i = 0; i < m_subsystems.size(); ++i)
    {
        if(typeid(T) == typeid(*m_subsystems[i].ptr))
            return m_subsystems[i].ptr->query_shared_ptr<T>();
    }

    return nullptr;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<typename T>
void context::validate_subsystem()
{
    static_assert(eastl::is_base_of<subsystem, T>::value, "Provided type does not implement subsystem");
}

XR_NAMESPACE_END(xr, extension)
//-----------------------------------------------------------------------------------------------------------