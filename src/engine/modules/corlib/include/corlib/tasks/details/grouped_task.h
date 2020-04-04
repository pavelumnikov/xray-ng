/*

  Copyright (c) 2020, Pavel Umnikov
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

#include "corlib/tasks/details/task_desc.h"
#include "corlib/tasks/details/task_group.h"

//------------------------------------------------------------------------------
namespace xr::tasks
{
class fiber_context;
} // namespace xr::tasks
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
namespace xr::tasks::details
{

//------------------------------------------------------------------------------
struct grouped_task
{
    fiber_context* awaiting_fiber { nullptr };
    fiber_context* parent_fiber { nullptr };
    task_desc desc {};
    task_group group {};

    grouped_task() = default;
    grouped_task(const task_desc& _desc, task_group _group);

    XR_DECLARE_DEFAULT_MOVE_ASSIGNMENT(grouped_task);
    XR_DECLARE_DEFAULT_COPY_ASSIGNMENT(grouped_task);
}; // struct grouped_task

//------------------------------------------------------------------------------
/**
 */
inline grouped_task::grouped_task(const task_desc& _desc, task_group _group)
    : awaiting_fiber { nullptr }
    , parent_fiber { nullptr }
    , desc { _desc }
    , group { _group }
{}

} // namespace xr::tasks::details
//------------------------------------------------------------------------------
