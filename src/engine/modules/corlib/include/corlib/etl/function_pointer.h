/*

  Copyright (c) 2019, Pavel Umnikov
  All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:

  * Redistributions of source code must retain the above copyright notice,
    this list of conditions and the following disclaimer.
  * Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.

  THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND ANY
  EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  DISCLAIMED. IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE FOR ANY
  DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
  OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
  DAMAGE.

*/

#pragma once

//------------------------------------------------------------------------------
namespace xr::etl
{

//------------------------------------------------------------------------------
template<typename RetType, typename... Args>
struct function_signature final
{
    using value_type = RetType;
    using type = typename RetType(Args...);
};

//------------------------------------------------------------------------------
template<typename RetType, typename... Args>
using function_signature_t = typename function_signature<RetType, Args...>::type;

//------------------------------------------------------------------------------
template<typename RetType, typename... Args>
struct function_pointer final
{
    using value_type = RetType;
    using type = typename value_type(*)(Args...);
};

//------------------------------------------------------------------------------
template<typename RetType, typename... Args>
using function_pointer_t = typename function_pointer<RetType, Args...>::type;


//------------------------------------------------------------------------------
template<typename ClassType, typename RetType, typename... Args>
struct member_function_pointer final
{
    using value_type = RetType;
    using type = typename RetType(ClassType::*)(Args...);
};

//------------------------------------------------------------------------------
template<typename ClassType, typename RetType, typename... Args>
using member_function_pointer_t = 
    typename member_function_pointer<ClassType, RetType, Args...>::type;

//------------------------------------------------------------------------------
template<typename ClassType, typename RetType, typename... Args>
struct const_member_function_pointer final
{
    using value_type = RetType;
    using type = typename value_type(ClassType::*)(Args...) const;
};

//------------------------------------------------------------------------------
template<typename ClassType, typename RetType, typename... Args>
using const_member_function_pointer_t =
typename const_member_function_pointer<ClassType, RetType, Args...>::type;

//------------------------------------------------------------------------------
using action_func = function_pointer_t<void>;
using prediction_func = function_pointer_t<bool>;

} // namespace xr::etl
//------------------------------------------------------------------------------