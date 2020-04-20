// This file is a part of xray-ng engine
//

#pragma once

//-----------------------------------------------------------------------------------------------------------
namespace xr::utils
{

//-----------------------------------------------------------------------------------------------------------
template< typename RetType, typename... Args >
class delegated_function
{
public:
    using return_type = RetType;

    template< typename FuncType >
    static delegated_function< RetType, Args... > bind(FuncType&& function);

    delegated_function();
    bool operator==(delegated_function const& rhs) const;
    bool operator!=(delegated_function const& rhs) const;
    return_type operator()(Args&&... arguments) const;

private:
    typedef return_type(*stubbed_type)(Args...); //!< Method pointer typedef
    stubbed_type m_function;
}; // class delegated_function

template< typename RetType, typename... Args >
inline delegated_function<RetType, Args...>::delegated_function()
    : m_function { nullptr }
{}

template< typename RetType, typename... Args >
bool delegated_function<RetType, Args...>::operator==(delegated_function const& rhs) const
{
    return m_function == rhs.m_function;
}

template< typename RetType, typename... Args >
bool delegated_function<RetType, Args...>::operator!=(delegated_function const& rhs) const
{
    return m_function != rhs.m_function;
}

template< typename RetType, typename... Args >
template< typename FuncType >
inline delegated_function< RetType, Args... > 
delegated_function< RetType, Args... >::bind(FuncType&& function)
{
    delegated_function< RetType, Args... > delegate;
    delegate.m_function = function;
    return delegate;
}

template< typename RetType, typename... Args >
inline typename delegated_function< RetType, Args... >::return_type
delegated_function< RetType, Args... >::operator()(Args&&... argument) const
{
    return this->m_function(argument...);
}


template< typename Return, typename Class, typename... Args >
class delegated_method
{
public:
    using return_type = Return;
    using class_type = Class;

    template< typename FuncType >
    static delegated_method<return_type, class_type, Args...> bind(FuncType&& function);

    delegated_method();
    bool operator==(delegated_method const& rhs) const;
    bool operator!=(delegated_method const& rhs) const;
    return_type operator()(class_type* ptr, Args&&... arguments) const;

private:
    typedef return_type(class_type::* stubbed_type)(Args...); //!< Method pointer typedef
    stubbed_type m_function;
};

template< typename Return, typename Class, typename... Args >
inline delegated_method< Return, Class, Args... >::delegated_method()
    : myFunction(nullptr)
{}

template< typename Return, typename Class, typename... Args >
bool delegated_method<Return, Class, Args...>::operator==(delegated_method const& rhs) const
{
    return m_function == rhs.m_function;
}

template< typename Return, typename Class, typename... Args >
bool delegated_method<Return, Class, Args...>::operator!=(delegated_method const& rhs) const
{
    return m_function != rhs.m_function;
}

template< typename Return, typename Class, typename... Args >
template< typename FuncType >
inline delegated_method< Return, Class, Args... > 
delegated_method< Return, Class, Args... >::bind(FuncType&& function)
{
    delegated_method< Return, Class, Args... > delegate;
    delegate.m_function = function;
    return delegate;
}

template< typename Return, typename Class, typename... Args >
inline typename delegated_method< Return, Class, Args... >::return_type
delegated_method< Return, Class, Args... >::operator()(class_type* ptr, Args&&... argument) const
{
    class_type* class_ptr = static_cast<class_type*>(ptr);
    return (class_ptr->*m_function)(argument...);
}

} // namespace xr::utils
//-----------------------------------------------------------------------------------------------------------
