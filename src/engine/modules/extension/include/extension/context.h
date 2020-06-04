// This file is a part of xray-ng engine
//

#pragma once

#include "extension/subsystem.h"
#include "corlib/utils/ptr.h"
#include "corlib/utils/unordered_map.h"
#include "corlib/utils/vector.h"

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, extension)

//-----------------------------------------------------------------------------------------------------------
class context
{
public:
    context(memory::base_allocator& alloc);
    ~context();

    template<typename T, typename ... Args>
    void register_subsystem(Args&& ... args);

    template<typename T>
    utils::shared_ptr<T> get_subsystem() const;

    // Initialize subsystems
    bool initialize_async(tasks::execution_context& ctx);
    // Tick
    void tick_async(tasks::execution_context& ctx, float delta = 0.0f);

private:
    using subsystems_container = utils::vector<utils::shared_ptr<subsystem>>;

    template<typename T>
    static void validate_subsystem();

    memory::base_allocator& m_allocator;
    memory::proxy::eastl_proxy_allocator m_proxy_allocator;
    subsystems_container m_subsystems;
}; // class context

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<typename T, typename ... Args>
inline void context::register_subsystem(Args&& ... args)
{
    validate_subsystem<T>();
    auto sub = utils::make_shared_ptr<T>(m_allocator, this, eastl::forward<Args&&>(args)...);
    m_subsystems.emplace_back(eastl::static_pointer_cast<subsystem>(sub));
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
        if(typeid(T) == typeid(*m_subsystems[i]))
            return m_subsystems[i]->query_shared_ptr<T>();
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