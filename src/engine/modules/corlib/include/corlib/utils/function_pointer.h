// This file is a part of xray-ng engine
//

#pragma once

//-----------------------------------------------------------------------------------------------------------
namespace xr::utils
{

//-----------------------------------------------------------------------------------------------------------
template<typename RetType, typename... Args>
struct function_signature final
{
    using value_type = RetType;
    using type = typename RetType(Args...);
};

//-----------------------------------------------------------------------------------------------------------
template<typename RetType, typename... Args>
using function_signature_t = typename function_signature<RetType, Args...>::type;

//-----------------------------------------------------------------------------------------------------------
template<typename RetType, typename... Args>
struct function_pointer final
{
    using value_type = RetType;
    using type = typename value_type(*)(Args...);
};

//-----------------------------------------------------------------------------------------------------------
template<typename RetType, typename... Args>
using function_pointer_t = typename function_pointer<RetType, Args...>::type;


//-----------------------------------------------------------------------------------------------------------
template<typename ClassType, typename RetType, typename... Args>
struct member_function_pointer final
{
    using value_type = RetType;
    using type = typename RetType(ClassType::*)(Args...);
};

//-----------------------------------------------------------------------------------------------------------
template<typename ClassType, typename RetType, typename... Args>
using member_function_pointer_t = 
    typename member_function_pointer<ClassType, RetType, Args...>::type;

//-----------------------------------------------------------------------------------------------------------
template<typename ClassType, typename RetType, typename... Args>
struct const_member_function_pointer final
{
    using value_type = RetType;
    using type = typename value_type(ClassType::*)(Args...) const;
};

//-----------------------------------------------------------------------------------------------------------
template<typename ClassType, typename RetType, typename... Args>
using const_member_function_pointer_t =
typename const_member_function_pointer<ClassType, RetType, Args...>::type;

//-----------------------------------------------------------------------------------------------------------
using action_func = function_pointer_t<void>;
using prediction_func = function_pointer_t<bool>;

} // namespace xr::utils
//-----------------------------------------------------------------------------------------------------------
