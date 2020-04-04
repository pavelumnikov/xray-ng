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

#include "corlib/tasks/details/task_bucket.h"
#include "corlib/tasks/details/grouped_task.h"
#include "corlib/etl/containers/array_view.h"

//------------------------------------------------------------------------------
namespace xr::tasks::details
{

//------------------------------------------------------------------------------
template<typename TTask>
struct grouped_task_selector
{
    inline grouped_task get_grouped_task(task_group group, const TTask* src)
    {
        task_desc desc(TTask::task_entry_pfn, (pvoid)src,
            TTask::stack_requirements, TTask::task_priority);

#ifdef XR_INSTRUMENTED_BUILD
        desc.debug_id = TTask::debug_id;
        desc.debug_color = TTask::debug_color;
#endif // XR_INSTRUMENTED_BUILD

        return grouped_task(desc, group);
    }
}; // struct grouped_task_selector

//------------------------------------------------------------------------------
/**
 * Distributes task to threads, for example:
 * | Task1 | Task2 | Task3 | Task4 | Task5 | Task6 |
 * ThreadCount = 4
 * Thread0: Task1, Task5
 * Thread1: Task2, Task6
 * Thread2: Task3
 * Thread3: Task4
 */
template<typename TTask>
inline bool distibute_descriptions(task_group group, const TTask* tasks,
    etl::containers::array_view<grouped_task>& descriptions, etl::containers::array_view<task_bucket>& buckets)
{
    grouped_task_selector<TTask> helper {};

    size_t index = 0;
    for(size_t bucket_index = 0; (bucket_index < buckets.size()) && (index < descriptions.size()); ++bucket_index)
    {
        size_t bucketStartIndex = index;
        for(size_t i = bucket_index; i < descriptions.size(); i += buckets.size())
            descriptions[index++] = eastl::move(helper.get_grouped_task(group, &tasks[i]));

        buckets[bucket_index] = eastl::move(task_bucket(&descriptions[bucketStartIndex], index - bucketStartIndex));
    }

    XR_DEBUG_ASSERTION_MSG(index == descriptions.size(), "Sanity check");
    return index > 0;
}

} // namespace xr::tasks::details
//------------------------------------------------------------------------------