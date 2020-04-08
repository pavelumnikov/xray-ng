// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/etl/containers/intrusive_list_node.h"
#include "corlib/etl/function_pointer.h"
#include "EASTL/functional.h"
#include "EASTL/fixed_function.h"

//------------------------------------------------------------------------------
namespace xr::etl
{

//------------------------------------------------------------------------------
template<typename ... Args>
class XR_NON_VIRTUAL base_slot : 
    public containers::intrusive_list_node<slot<Args...>>
{
public:
    virtual ~base_slot() {};
    virtual void invoke(Args... args) = 0;
};

//------------------------------------------------------------------------------
template<size_t N, typename ... Args>
class fixed_slot : public base_slot<Args...>
{
public:
    using callback_function =
        eastl::fixed_function<N, function_signature_t<void, Args...>>;
};

//------------------------------------------------------------------------------
template <typename ... Args>
class slot final : public base_slot<Args...>
{
public:
    using callback_function = 
        eastl::function<function_signature_t<void, Args...>>;

    // Default constructor
    slot() noexcept;
    explicit slot(callback_function const& s) noexcept;

    XR_DECLARE_DEFAULT_DESTRUCTOR(slot);
    XR_DECLARE_DELETE_COPY_ASSIGNMENT(slot);
    XR_DECLARE_DEFAULT_MOVE_ASSIGNMENT(slot);

    void set(callback_function const& s) noexcept;
    virtual void invoke(Args... args) override;

private:
    callback_function m_callback;
}; // class slot


//------------------------------------------------------------------------------
/**
*/
template <typename ... Args>
slot<Args...>::slot() noexcept
    : m_callback { nullptr }
{}

//------------------------------------------------------------------------------
/**
*/
template <typename ... Args>
slot<Args...>::slot(callback_function const& s) noexcept
    : m_callback { s }
{}

//------------------------------------------------------------------------------
/**
*/
template <typename ... Args>
void slot<Args...>::set(callback_function const& s) noexcept
{
    XR_DEBUG_ASSERTION_MSG(!this->m_callback, 
        "Callback function can be set only once");

    m_callback = s;
}

//------------------------------------------------------------------------------
/**
*/
template<typename ... Args>
void slot<Args...>::invoke(Args... args) noexcept
{
    XR_DEBUG_ASSERTION_MSG(this->m_callback, 
        "Callback function is not defined for slot");
    m_callback(args...);
}


//------------------------------------------------------------------------------
/**
 * Signal is a template type providing an interface for arbitrary callback
 * lists.
 * A signal type needs to be declared with the function signature of its
 * callbacks,
 * and optionally a return result collector class type.
 * Signal callbacks can be added with operator+= to a signal and removed with
 * operator-=, using
 * a callback connection ID return by operator+= as argument.
 * The callbacks of a signal are invoked with the emit() method and arguments
 * according to the signature.
 * The result returned by emit() depends on the signal collector class. By
 * default, the result of
 * the last callback is returned from emit(). Collectors can be implemented to
 * accumulate callback
 * results or to halt a running emissions in correspondence to callback results.
 * The signal implementation is safe against recursion, so callbacks may be
 * removed and
 * added during a signal emission and recursive emit() calls are also safe.
 * The overhead of an unused signal is intentionally kept very low, around the
 * size of a single pointer.
 * Note that the Signal template types is non-copyable.
 */
template <typename ... Args>
class signal final
{
public:
    using slot_node = slot<Args...>;

    // Default constructor
    signal() noexcept = default;
    // Signal constructor, supports a default callback as argument.
    explicit signal(slot_node const& s) noexcept;
    // Default destructor
    ~signal();

    XR_DECLARE_DELETE_COPY_ASSIGNMENT(signal);
    XR_DECLARE_DEFAULT_MOVE_ASSIGNMENT(signal);

    // connects a slot to the signal.
    void connect(slot_node& s) noexcept;
    // disconnects a previously connected function
    void disconnect(slot_node& s) noexcept;
    // disconnects all previously connected functions
    void disconnect_all() noexcept;

    // calls all connected slots
    void emit(Args... args) noexcept;

private:
    slot_node m_slot;
}; // class signal


//------------------------------------------------------------------------------
/**
*/
template <typename ... Args>
signal<Args...>::signal(slot_node const& s) noexcept
{
    m_slot.append_after_this(s);
}

//------------------------------------------------------------------------------
/**
*/
template <typename ... Args>
signal<Args...>::~signal()
{
    disconnect_all();
}

//------------------------------------------------------------------------------
/**
*/
template <typename ... Args>
void signal<Args...>::connect(slot_node& s) noexcept
{
    m_slot.append_after_this(s);
}

//------------------------------------------------------------------------------
/**
*/
template <typename ... Args>
void signal<Args...>::disconnect(slot_node& s) noexcept
{
    s.remove_this();
}

//------------------------------------------------------------------------------
/**
*/
template <typename ... Args>
void signal<Args...>::disconnect_all() noexcept
{
    auto s = m_slot->next();
    if(s)
        s.remove_this();
}

//------------------------------------------------------------------------------
/**
*/
template <typename ... Args>
void signal<Args...>::emit(Args... args) noexcept
{
    XR_DEBUG_ASSERTION(m_slot.is_head());
    for(slot_node* current = m_slot.next();
        current != nullptr; current = current->next())
    {
        current->invoke(args...);
    }
}

} // namespace xr::etl
//------------------------------------------------------------------------------