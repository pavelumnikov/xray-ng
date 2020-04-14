// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/tasks/details/task_bucket.h"
#include "corlib/tasks/details/grouped_task.h"
#include "corlib/etl/containers/array_view.h"

//-----------------------------------------------------------------------------------------------------------
namespace xr::tasks::details
{

//-----------------------------------------------------------------------------------------------------------
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

//-----------------------------------------------------------------------------------------------------------
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
    etl::array_view<grouped_task>& descriptions, etl::array_view<task_bucket>& buckets)
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
//-----------------------------------------------------------------------------------------------------------
