// This file is a part of xray-ng engine
//

#include "scheduler.h"
#include "fiber.h"
#include "corlib/memory/memory_allocator_base.h"
#include "corlib/utils/static_vector.h"
#include "corlib/threading/atomic_backoff.h"
#include "corlib/tasks/details/work_distribution.h"
#include <string.h> // for memset
#include <stdio.h> // for _snwprintf

// Enable low latency experimental wait code path.
// Low latency hybrid wait works better on consoles, but a little worse on PC.
// #define XR_EXPERIMENTAL_WAIT (1)

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, tasks, details)

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<>
struct grouped_task_selector<fiber_context*>
{
    inline grouped_task get_grouped_task(task_group group, fiber_context* const* src)
    {
        XR_UNREFERENCED_PARAMETER(group);
        XR_DEBUG_ASSERTION_MSG(group == task_group::assign_from_context,
            "Group must be assigned from context");

        fiber_context* fiber_context = *src;
        XR_DEBUG_ASSERTION_MSG(fiber_context->current_task.required_stack == fiber_context->required_stack,
            "Sanity check failed");

        grouped_task task { fiber_context->current_task, fiber_context->current_group };
        task.awaiting_fiber = fiber_context;
        return task;
    }
};

XR_NAMESPACE_END(xr, tasks, details)
//-----------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, tasks)

constexpr size_t scheduler_stack_size = XR_MEGABYTES_TO_BYTES(1); // 1Mb
constexpr size_t standard_fiber_stack_size = XR_KILOBYTES_TO_BYTES(256); // 256Kb
constexpr size_t reserve_standard_fiber_stack_size = standard_fiber_stack_size / 2;
constexpr size_t extended_fiber_stack_size = XR_MEGABYTES_TO_BYTES(1); // 1Mb
constexpr size_t reserve_extended_fiber_stack_size = extended_fiber_stack_size / 2;

//-----------------------------------------------------------------------------------------------------------
/**
 */
#ifdef XR_INSTRUMENTED_BUILD
task_scheduler::task_scheduler(memory::base_allocator& alloc, uint32_t workerThreadsCount, base_profiler_event_listener* listener)
#else
task_scheduler::task_scheduler(memory::base_allocator& alloc, uint32_t workerThreadsCount)
#endif
    : m_aligned_allocator { alloc }
    , m_round_robin_thread_index { 0 }
    , m_started_threads_count { 0 }
{

#ifdef XR_INSTRUMENTED_BUILD
    m_profiler_event_listener = listener;
#endif

    if(workerThreadsCount != 0)
        m_threads_count = eastl::clamp<uint32_t>(workerThreadsCount, 1, max_thread_count);
    else
        m_threads_count = eastl::clamp<uint32_t>(sys::core_count() - 1, 1, max_thread_count);

    uint32_t fiber_index = 0;
    // create fiber pool (fibers with standard stack size)
    for(uint32_t i = 0; i < max_standard_fibers_count; i++)
    {
        fiber_context& context = m_standard_fiber_contexts[i];
        context.system_fiber.create(standard_fiber_stack_size, 
            reserve_standard_fiber_stack_size, fiber_main, &context);

        context.fiber_index = fiber_index;
        bool res = m_standard_fibers_available.enqueue(&context);
        XR_UNREFERENCED_PARAMETER(res);
        XR_DEBUG_ASSERTION_MSG(res == true, "Can't add fiber to storage");
        fiber_index++;
    }

    // create fiber pool (fibers with extended stack size)
    for(uint32_t i = 0; i < max_extended_fibers_count; i++)
    {
        fiber_context& context = m_extended_fiber_contexts[i];
        context.system_fiber.create(extended_fiber_stack_size, 
            reserve_extended_fiber_stack_size, fiber_main, &context);

        context.fiber_index = fiber_index;
        bool res = m_extended_fibers_available.enqueue(&context);
        XR_UNREFERENCED_PARAMETER(res);
        XR_DEBUG_ASSERTION_MSG(res == true, "Can't add fiber to storage");
        fiber_index++;
    }

#ifdef XR_INSTRUMENTED_BUILD
    notify_fibers_created(kMaxStandardFibersCount + kMaxExtendedFibersCount);
#endif

    for(uint16_t i = 0; i < task_group::max_groups_count; i++)
    {
        if(i != task_group::default_group)
        {
            bool res = m_available_groups.enqueue(task_group(i));
            XR_UNREFERENCED_PARAMETER(res);
            XR_DEBUG_ASSERTION_MSG(res == true, "Can't add group to storage");
        }
    }

#if defined(DEBUG)
    m_group_stats[task_group::default_group].set_debug_is_free(false);
#endif // defined(DEBUG)

    // create worker thread pool
    uint32_t total_threads_count = get_workers_count();

#ifdef XR_INSTRUMENTED_BUILD
    notify_threads_created(total_thread_count);
#endif

    m_thread_context = XR_ALLOCATE_OBJECT_ARRAY_T(m_aligned_allocator, 
        details::thread_context, total_threads_count, "thread contexts for threads");

    for(uint32_t i = 0; i < total_threads_count; i++)
    {
        details::thread_context& context = m_thread_context[i];
        memory::call_emplace_construct(&context, m_aligned_allocator);

        uint32_t thread_index = static_cast<uint32_t>(i);
        context.set_thread_index(thread_index);
        context.current_scheduler = this;

        auto priority = sys::thread_priority::medium;

        wchar_t worker_name[16];
        _snwprintf_s(worker_name, eastl::size(worker_name), L"task_worker %u", i);
        context.current_thread = sys::spawn_thread(worker_thread_main, &context,
            worker_name, priority, scheduler_stack_size, thread_index);
    }
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
void task_scheduler::join_worker_threads()
{
    size_t total_threads_count = get_workers_count();
    for(size_t i = 0; i < total_threads_count; i++)
    {
        threading::atomic_store_rel(m_thread_context[i].state, (uint32_t)details::thread_state::EXIT);
        m_thread_context[i].has_new_tasks_event.set(true);
    }

    sys::thread_handle thread_ids[64];
    for(size_t i = 0; i < total_threads_count; i++)
        thread_ids[i] = m_thread_context[i].current_thread;

    bool result = sys::wait_threads(thread_ids, total_threads_count);
    XR_UNREFERENCED_PARAMETER(result);

    for(size_t i = 0; i < total_threads_count; i++)
    {
        details::thread_context& context = m_thread_context[i];
        memory::call_destruct(&context);
    }
    XR_DEALLOCATE_MEMORY(m_aligned_allocator, m_thread_context);

    threading::atomic_store_rel<uint32_t>(m_threads_count, 0);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
task_scheduler::~task_scheduler()
{
    if(get_workers_count() > 0)
        join_worker_threads();
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
fiber_context* task_scheduler::request_fiber_context(details::grouped_task& t)
{
    auto fiber_ctx = t.awaiting_fiber;
    if(fiber_ctx)
    {
        t.awaiting_fiber = nullptr;
        return fiber_ctx;
    }

    auto required_stack = t.desc.required_stack;

    fiber_ctx = nullptr;
    bool res = false;
    XR_UNREFERENCED_PARAMETER(res);
    switch(required_stack)
    {
        case task_stack_request::small_stack:
            res = m_standard_fibers_available.dequeue(fiber_ctx);
            XR_UNREFERENCED_PARAMETER(res);
            XR_DEBUG_ASSERTION_MSG(res, "Can't get more standard fibers!");
            break;

        case task_stack_request::huge_stack:
            res = m_extended_fibers_available.dequeue(fiber_ctx);
            XR_UNREFERENCED_PARAMETER(res);
            XR_DEBUG_ASSERTION_MSG(res, "Can't get more extended fibers!");
            break;

        default:
            XR_DEBUG_ASSERTION_MSG(false, "Unknown stack requrements");
    }

    XR_DEBUG_ASSERTION_MSG(fiber_ctx != nullptr, "Can't get more fibers. Too many tasks in flight simultaneously?");
    fiber_ctx->current_task = t.desc;
    fiber_ctx->current_group = t.group;
    fiber_ctx->parent_fiber = t.parent_fiber;
    fiber_ctx->required_stack = required_stack;
    return fiber_ctx;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
void task_scheduler::release_fiber_context(fiber_context*&& fiber_ctx)
{
    XR_DEBUG_ASSERTION_MSG(fiber_ctx, "Can't release nullptr Fiber. fiber_ctx is nullptr");

    auto required_stack = fiber_ctx->required_stack;
    fiber_ctx->reset();

    XR_DEBUG_ASSERTION_MSG(fiber_ctx != nullptr, "Fiber context can't be nullptr");

    bool res = false;
    XR_UNREFERENCED_PARAMETER(res);
    switch(required_stack)
    {
        case task_stack_request::small_stack:
            res = m_standard_fibers_available.enqueue(std::move(fiber_ctx));
            break;

        case task_stack_request::huge_stack:
            res = m_extended_fibers_available.enqueue(std::move(fiber_ctx));
            break;

        default:
            XR_DEBUG_ASSERTION_MSG(false, "Unknown stack requrements");
    }

    XR_UNREFERENCED_PARAMETER(res);
    XR_DEBUG_ASSERTION_MSG(res != false, "Can't return fiber to storage");
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
fiber_context* task_scheduler::execute_task(details::thread_context& thread_ctx, fiber_context* fiber_ctx)
{
    XR_DEBUG_ASSERTION_MSG(thread_ctx.current_thread_id == sys::current_thread_id(),
        "Thread context sanity check failed");

    XR_DEBUG_ASSERTION_MSG(fiber_ctx, "Invalid fiber context");
    XR_DEBUG_ASSERTION_MSG(fiber_ctx->current_task.is_valid(), "Invalid task");

    // Set actual thread context to fiber
    fiber_ctx->set_thread_context(&thread_ctx);

    // Update task status
    fiber_ctx->set_status(fiber_task_status::RUNNED);

    XR_DEBUG_ASSERTION_MSG(fiber_ctx->get_thread_context()->current_thread_id == sys::current_thread_id(),
        "Thread context sanity check failed");

#ifdef XR_INSTRUMENTED_BUILD
    thread_ctx.notify_task_execute_state_changed(XR_SYSTEM_TASK_COLOR, XR_SYSTEM_TASK_NAME, task_execute_state::stop, XR_SYSTEM_FIBER_INDEX);
#endif

    // Run current task code
    fiber::switch_to(thread_ctx.scheduler_fiber, fiber_ctx->system_fiber);

#ifdef XR_INSTRUMENTED_BUILD
    thread_ctx.notify_task_execute_state_changed(XR_SYSTEM_TASK_COLOR, XR_SYSTEM_TASK_NAME, task_execute_state::start, XR_SYSTEM_FIBER_INDEX);
#endif

    // If task was done
    auto task_status = fiber_ctx->get_status();
    if(task_status == fiber_task_status::FINISHED)
    {
        task_group group = fiber_ctx->current_group;
        task_group_description& group_desc = thread_ctx.current_scheduler->get_group_desc(group);

        // Update group status
        int32_t group_task_count = group_desc.decrement();
        XR_DEBUG_ASSERTION_MSG(group_task_count >= 0, "Sanity check failed!");
        if(group_task_count == 0)
            fiber_ctx->current_group = task_group::invalid;

        // Update total task count
        int32_t all_group_task_count = thread_ctx.current_scheduler->m_all_groups.decrement();
        XR_UNREFERENCED_PARAMETER(all_group_task_count);
        XR_DEBUG_ASSERTION_MSG(all_group_task_count >= 0, "Sanity check failed!");

        fiber_context* parent_fiber_context = fiber_ctx->parent_fiber;
        if(parent_fiber_context != nullptr)
        {
            size_t children_fibers_count = threading::atomic_dec_fetch_seq(parent_fiber_context->children_fibers_count);
            XR_DEBUG_ASSERTION_MSG(children_fibers_count >= 0, "Sanity check failed!");

            if(children_fibers_count == 0)
            {
                // This is a last subtask. Restore parent task
                XR_DEBUG_ASSERTION_MSG(thread_ctx.current_thread_id == sys::current_thread_id(),
                    "Thread context sanity check failed");
                XR_DEBUG_ASSERTION_MSG(parent_fiber_context->get_thread_context() == nullptr,
                    "Inactive parent should not have a valid thread context");

                // WARNING!! Thread context can changed here! Set actual current thread context.
                parent_fiber_context->set_thread_context(&thread_ctx);

                XR_DEBUG_ASSERTION_MSG(parent_fiber_context->get_thread_context()->current_thread_id == sys::current_thread_id(),
                    "Thread context sanity check failed");

                // All subtasks is done.
                // Exiting and return parent fiber to scheduler
                return parent_fiber_context;
            }
            else
            {
                // Other subtasks still exist
                // Exiting
                return nullptr;
            }
        }
        else
        {
            // Task is finished and no parent task
            // Exiting
            return nullptr;
        }
    }

    XR_DEBUG_ASSERTION_MSG(task_status != fiber_task_status::RUNNED, "Incorrect task status");
    return nullptr;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
void task_scheduler::fiber_main(void* user_data)
{
    fiber_context& fiber_ctx = *reinterpret_cast<fiber_context*>(user_data);
    for(;;)
    {
        XR_DEBUG_ASSERTION_MSG(fiber_ctx.current_task.is_valid(), "Invalid task in fiber context");
        XR_DEBUG_ASSERTION_MSG(fiber_ctx.get_thread_context(), "Invalid thread context");
        XR_DEBUG_ASSERTION_MSG(fiber_ctx.get_thread_context()->current_thread_id == sys::current_thread_id(),
            "Thread context sanity check failed");

#ifdef XR_INSTRUMENTED_BUILD
        fiber_ctx.fiber.SetName(MT_SYSTEM_TASK_FIBER_NAME);
        fiber_ctx.get_thread_context()->notify_task_execute_state_changed(fiber_ctx.current_task.debug_color,
            fiber_ctx.current_task.debug_id, task_execute_state::start, fiber_ctx.fiber_index);
#endif

        pvoid user_data = fiber_ctx.current_task.user_data;
        fiber_ctx.current_task.task_func(fiber_ctx, user_data);
        fiber_ctx.set_status(fiber_task_status::FINISHED);

#ifdef XR_INSTRUMENTED_BUILD
        fiber_ctx.fiber.set_name(MT_SYSTEM_TASK_FIBER_NAME);
        fiber_ctx.get_thread_context()->notify_task_execute_state_changed(fiber_ctx.current_task.debug_color,
            fiber_ctx.current_task.debug_id, task_execute_state::stop, fiber_ctx.fiber_index);
#endif

        details::thread_context& thread_ctx = *fiber_ctx.get_thread_context();
        fiber::switch_to(fiber_ctx.system_fiber, thread_ctx.scheduler_fiber);
    }

}

//-----------------------------------------------------------------------------------------------------------
/**
 */
bool task_scheduler::try_steal_task(details::thread_context& thread_context, details::grouped_task& task)
{
    auto workers_count = thread_context.current_scheduler->get_workers_count();
    auto victim_index = thread_context.random.get();

    for(uint32_t attempt = 0; attempt < workers_count; attempt++)
    {
        uint32_t index = victim_index % workers_count;
        if(index == thread_context.current_worker_index)
        {
            victim_index++;
            index = victim_index % workers_count;
        }

        auto& victim_context = thread_context.current_scheduler->m_thread_context[index];
        if(victim_context.queue.try_pop_newest(task))
            return true;

        victim_index++;
    }

    return false;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
uint32_t task_scheduler::worker_thread_main(void* user_data)
{
    details::thread_context& context = *reinterpret_cast<details::thread_context*>(user_data);
    XR_DEBUG_ASSERTION_MSG(context.current_scheduler, "Task scheduler must be not null!");
    context.current_thread_id = sys::current_thread_id();
    context.scheduler_fiber.create_from_thread_and_run(scheduler_fiber_main, user_data);
    return 0;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
void task_scheduler::scheduler_fiber_wait(void* user_data)
{
    wait_context_desc& wait_context = *reinterpret_cast<wait_context_desc*>(user_data);
    details::thread_context& context = *wait_context.thread_ctx;
    XR_DEBUG_ASSERTION_MSG(context.current_scheduler, "Task scheduler must be not null!");
    XR_DEBUG_ASSERTION_MSG(wait_context.wait_counter, "Wait counter must be not null!");

#ifdef XR_INSTRUMENTED_BUILD
    context.notify_temporary_worker_thread_join();

    context.notify_wait_started();
    context.notify_task_execute_state_changed(XR_SYSTEM_TASK_COLOR, XR_SYSTEM_TASK_NAME, task_execute_state::start, XR_SYSTEM_FIBER_INDEX);
#endif

    sys::tick time_out = sys::now_milliseconds() + (wait_context.wait_time_ms * 1000U);
    threading::default_atomic_backoff backoff {};

    for(;;)
    {
        if(!scheduler_fiber_step(context))
            backoff.pause();
        else
            backoff.reset();

        uint32_t group_task_count = threading::atomic_fetch_acq(*wait_context.wait_counter);
        if(group_task_count == 0)
        {
            wait_context.exit_code = 0;
            break;
        }

        sys::tick time_now = sys::now_milliseconds();
        if(time_now >= time_out)
        {
            wait_context.exit_code = 1;
            break;
        }
    }

#ifdef XR_INSTRUMENTED_BUILD
    context.notify_task_execute_state_changed(XR_SYSTEM_TASK_COLOR, XR_SYSTEM_TASK_NAME, task_execute_state::stop, XR_SYSTEM_FIBER_INDEX);
    context.notify_wait_finished();

    context.notify_temporary_worker_thread_leave();
#endif
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
void task_scheduler::scheduler_fiber_main(void* user_data)
{
    details::thread_context& context = *(details::thread_context*)(user_data);
    XR_DEBUG_ASSERTION_MSG(context.current_scheduler, "Task scheduler must be not null!");

#ifdef XR_INSTRUMENTED_BUILD
    context.notify_thread_created(context.worker_index);
#endif

    uint32_t total_threads_count = threading::atomic_fetch_acq(context.current_scheduler->m_threads_count);
    (void)threading::atomic_fetch_inc_seq(context.current_scheduler->m_started_threads_count);

    // simple spinlock until all threads is started and initialized
    for(;;)
    {
        uint32_t initialized_threads_count =
            threading::atomic_fetch_acq(context.current_scheduler->m_started_threads_count);

        if(initialized_threads_count == total_threads_count)
            break;

        // sleep some time until all other thread initialized
        sys::yield(1);
    }

    XR_MEMORY_FULLCONSISTENCY_BARRIER;

#ifdef XR_INSTRUMENTED_BUILD
    context.notify_thread_started(context.worker_index);
    context.notify_task_execute_state_changed(XR_SYSTEM_TASK_COLOR, XR_SYSTEM_TASK_NAME, task_execute_state::start, XR_SYSTEM_FIBER_INDEX);
#endif

    while(threading::atomic_fetch_acq(context.state) != (uint32_t)details::thread_state::EXIT)
    {
        if(!scheduler_fiber_step(context))
        {
#ifdef XR_INSTRUMENTED_BUILD
            context.notify_thread_idle_started(context.worker_index);
#endif

#if XR_EXPERIMENTAL_WAIT
            SpinWait spinWait;

            for(;;)
            {
                // queue is empty and stealing attempt has failed.
                // Fast Spin Wait for new tasks
                if(spinWait.SpinOnce() >= SpinWait::YIELD_SLEEP0_THRESHOLD)
                {
                    // Fast Spin wait for new tasks has failed.
                    // Wait for new events using events
                    context.has_new_tasks_event.wait(20000);

                    spinWait.reset();

#ifdef XR_INSTRUMENTED_BUILD
                    context.notify_thread_idle_finished(context.worker_index);
#endif

                    break;
                }

                details::grouped_task task;
                if(context.queue.try_pop_oldest(task))
                {
#ifdef XR_INSTRUMENTED_BUILD
                    context.notify_thread_idle_finished(context.worker_index);
#endif

                    scheduler_fiber_process_task(context, task);

                    break;
                }

            }
#else
            // queue is empty and stealing attempt has failed.
            // Wait for new events using events
            context.has_new_tasks_event.wait_timeout(20000);

#ifdef XR_INSTRUMENTED_BUILD
            context.notify_thread_idle_finished(context.worker_index);
#endif

#endif

        }
    } // main thread loop

#ifdef XR_INSTRUMENTED_BUILD
    context.notify_task_execute_state_changed(XR_SYSTEM_TASK_COLOR, XR_SYSTEM_TASK_NAME, task_execute_state::stop, XR_SYSTEM_FIBER_INDEX);
    context.notify_thread_stopped(context.worker_index);
#endif
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
void task_scheduler::scheduler_fiber_process_task(details::thread_context& context, details::grouped_task& task)
{
#ifdef XR_INSTRUMENTED_BUILD
    bool isNewTask = (task.awaiting_fiber == nullptr);
#endif

    // There is a new task
    fiber_context* fiber_ctx = context.current_scheduler->request_fiber_context(task);
    XR_DEBUG_ASSERTION_MSG(fiber_ctx, "Can't get execution context from pool");
    XR_DEBUG_ASSERTION_MSG(fiber_ctx->current_task.is_valid(), "Task validation check failed");
    XR_DEBUG_ASSERTION_MSG(fiber_ctx->required_stack == task.desc.required_stack, "Sanity check failed");

    while(fiber_ctx)
    {
#ifdef XR_INSTRUMENTED_BUILD
        if(isNewTask)
        {
            //TODO:
            isNewTask = false;
        }
#endif
        // prevent invalid fiber resume from child tasks, before ExecuteTask is done
        threading::atomic_fetch_inc_seq(fiber_ctx->children_fibers_count);

        auto parent_fiber = execute_task(context, fiber_ctx);
        auto task_status = fiber_ctx->get_status();

        //release guard
        size_t children_fibers_count = threading::atomic_dec_fetch_seq(fiber_ctx->children_fibers_count);

        // Can drop fiber context - task is finished
        if(task_status == fiber_task_status::FINISHED)
        {
            XR_DEBUG_ASSERTION_MSG(children_fibers_count == 0, "Sanity check failed");
            context.current_scheduler->release_fiber_context(std::move(fiber_ctx));

            // If parent fiber is exist transfer flow control to parent fiber, if parent fiber is null, exit
            fiber_ctx = parent_fiber;
        }
        else
        {
            XR_DEBUG_ASSERTION_MSG(children_fibers_count >= 0, "Sanity check failed");

            // No subtasks here and status is not finished, this mean all subtasks already 
            // finished before parent return from ExecuteTask
            if(children_fibers_count == 0)
            {
                XR_DEBUG_ASSERTION_MSG(parent_fiber == nullptr, "Sanity check failed");
            }
            else
            {
                // If subtasks still exist, drop current task execution.
                // task will be resumed when last subtask finished.
                break;
            }

            // If task is yielded execution, get another task from queue.
            if(task_status == fiber_task_status::YIELDED)
            {
                // Task is yielded, add to tasks queue
                utils::array_view<details::grouped_task> buffer(context.desc_buffer, 1);
                utils::array_view<details::task_bucket> buckets(
                    XR_STACK_ALLOCATE_MEMORY(sizeof(details::task_bucket)), 1);

                fiber_context* yieldedTask = fiber_ctx;
                utils::static_vector<fiber_context*, 1> yielded_tasks_queue(1, yieldedTask);

                details::distibute_descriptions(task_group(task_group::assign_from_context),
                    yielded_tasks_queue.begin(), buffer, buckets);

                // add yielded task to scheduler
                context.current_scheduler->run_tasks_internal(buckets, nullptr, true);

                // ATENTION! yielded task can be already completed at this point

                break;
            }
        }
    } //while(fiber_ctx)
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
bool task_scheduler::scheduler_fiber_step(details::thread_context& context)
{
    details::grouped_task task;

    bool from_foreign_context = false;
    bool from_current_context = context.queue.try_pop_oldest(task);
    if(!from_current_context)
    {
        from_foreign_context = try_steal_task(context, task);
    }

    if(from_current_context || from_foreign_context)
    {
        scheduler_fiber_process_task(context, task);
        return true;
    }

    return false;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
void task_scheduler::run_tasks_internal(utils::array_view<details::task_bucket>& buckets, 
    fiber_context* parent_fiber, bool restored_from_awaiting)
{
#if XR_EXPERIMENTAL_WAIT
    // Early wakeup worker threads (worker thread spin wait for some time before sleep)
    int32 round_robin_index = m_round_robin_thread_index.LoadRelaxed();
    for(size_t i = 0; i < buckets.size(); ++i)
    {
        int bucket_index = ((round_robin_index + i) % m_threads_count.LoadRelaxed());
        details::thread_context& context = thread_ctx[bucket_index];
        context.has_new_tasks_event.set(true);
    }
#endif

    // This storage is necessary to calculate how many tasks we add to different groups
    uint32_t new_task_count_in_group[task_group::max_groups_count];
    // Default value is 0
    memset(&new_task_count_in_group[0], 0, sizeof(new_task_count_in_group));

    // Set parent fiber pointer
    // Calculate the number of tasks per group
    // Calculate total number of tasks
    size_t count = 0;
    for(size_t i = 0; i < buckets.size(); ++i)
    {
        details::task_bucket& bucket = buckets[i];
        for(size_t task_index = 0; task_index < bucket.count; task_index++)
        {
            details::grouped_task& task = bucket.tasks[task_index];
            task.parent_fiber = parent_fiber;

            int idx = task.group.get_valid_index();
            XR_DEBUG_ASSERTION_MSG(idx >= 0 && idx < task_group::max_groups_count, "Invalid index");
            new_task_count_in_group[idx]++;
        }

        count += bucket.count;
    }

    // Increments child fibers count on parent fiber
    if(parent_fiber)
        threading::atomic_fetch_add_seq(parent_fiber->children_fibers_count, count);

    if(restored_from_awaiting == false)
    {
        // Increase the number of active tasks in the group using data from temporary storage
        for(size_t i = 0; i < task_group::max_groups_count; i++)
        {
            int group_new_task_count = new_task_count_in_group[i];
            if(group_new_task_count > 0)
                m_group_stats[i].add((uint32_t)group_new_task_count);
        }

        // Increments all task in progress counter
        m_all_groups.add((uint32_t)count);
    }
    else
    {
        // If task's restored from await state, counters already in correct state
    }

    // add to thread queue
    for(size_t i = 0; i < buckets.size(); ++i)
    {
        uint32_t bucket_index = threading::atomic_fetch_inc_seq(m_round_robin_thread_index) % m_threads_count;
        details::thread_context& context = m_thread_context[bucket_index];
        details::task_bucket& bucket = buckets[i];

        for(;;)
        {
            XR_DEBUG_ASSERTION_MSG(bucket.count < (details::max_task_buffer_capacity - 1),
                "Sanity check failed. Too many tasks per one bucket.");

            bool res = context.queue.add(bucket.tasks, bucket.count);
            if(res == true) break;

            // Can't add new tasks onto the queue. Look like the job system is overloaded.
            // Wait some time and try again.
            // TODO: implement waiting until workers done using events.
            sys::yield(10);
        }

        context.has_new_tasks_event.set(true);
    }
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
signalling_bool task_scheduler::wait_group(task_group group, uint32_t milliseconds)
{
    XR_DEBUG_ASSERTION_MSG(!is_worker_thread(), "Can't use wait_group inside task.");
    task_group_description& group_desc = get_group_desc(group);

    // Early exit if not tasks in group
    if(!group_desc.get_task_count())
        return true;

    size_t bytes_count_for_desc_buffer = details::memory_requrements_for_desc_buffer;
    pvoid desc_buffer = XR_STACK_ALLOCATE_MEMORY(bytes_count_for_desc_buffer);

    details::thread_context context { desc_buffer };
    context.current_scheduler = this;
    context.set_thread_index(0xFFFFFFFF);
    context.current_thread_id = sys::current_thread_id();

    wait_context_desc wait_context {};
    wait_context.thread_ctx = &context;
    wait_context.wait_counter = &group_desc.get_wait_counter();
    wait_context.wait_time_ms = milliseconds;

    uint32_t next_slot = threading::atomic_fetch_inc_seq(m_next_awaiting_master_index);
    m_awaiting_masters[next_slot % m_awaiting_masters.size()] = sys::current_thread_id();
    context.scheduler_fiber.create_from_thread_and_run(scheduler_fiber_wait, &wait_context);

    XR_DEBUG_ASSERTION_MSG(m_awaiting_masters[next_slot % m_awaiting_masters.size()] == sys::current_thread_id(),
        "m_awaiting_masters array overflow");
    m_awaiting_masters[next_slot % m_awaiting_masters.size()] = 0;

    return (wait_context.exit_code == 0);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
signalling_bool task_scheduler::wait_all(uint32_t milliseconds)
{
    XR_DEBUG_ASSERTION_MSG(!is_worker_thread(), "Can't use wait_all inside Task.");
    // Early exit if not tasks in group
    if(!m_all_groups.get_task_count())
        return true;

    size_t bytes_count_for_desc_buffer = details::memory_requrements_for_desc_buffer;
    pvoid desc_buffer = XR_STACK_ALLOCATE_MEMORY(bytes_count_for_desc_buffer);

    details::thread_context context { desc_buffer };
    context.current_scheduler = this;
    context.set_thread_index(0xFFFFFFFF);
    context.current_thread_id = sys::current_thread_id();

    wait_context_desc wait_context {};
    wait_context.thread_ctx = &context;
    wait_context.wait_counter = &m_all_groups.get_wait_counter();
    wait_context.wait_time_ms = milliseconds;

    uint32_t next_slot = threading::atomic_fetch_inc_seq(m_next_awaiting_master_index);
    m_awaiting_masters[next_slot % m_awaiting_masters.size()] = sys::current_thread_id();
    context.scheduler_fiber.create_from_thread_and_run(scheduler_fiber_wait, &wait_context);

    XR_DEBUG_ASSERTION_MSG(m_awaiting_masters[next_slot % m_awaiting_masters.size()] == sys::current_thread_id(),
        "m_awaiting_masters array overflow");
    m_awaiting_masters[next_slot % m_awaiting_masters.size()] = 0;

    return wait_context.exit_code == 0;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
uint32_t task_scheduler::get_workers_count() const
{
    return m_threads_count;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
bool task_scheduler::is_worker_thread() const
{
    size_t threads_count = get_workers_count();
    for(size_t i = 0; i < threads_count; i++)
    {
        if(m_thread_context[i].current_thread_id == sys::current_thread_id())
            return true;
    }

    for(size_t i = 0; i < m_awaiting_masters.size(); i++)
    {
        if(m_awaiting_masters[i] == sys::current_thread_id())
            return true;
    }

    return false;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
task_group task_scheduler::create_group()
{
    XR_DEBUG_ASSERTION_MSG(!is_worker_thread(), "Can't use create_group inside Task.");

    task_group group;
    if(!m_available_groups.dequeue(group))
    {
        // TODO: report group pool is empty
    }

    int idx = group.get_valid_index();
    XR_UNREFERENCED_PARAMETER(idx);
    XR_DEBUG_ASSERTION_MSG(m_group_stats[idx].get_debug_is_free() == true, "Bad logic!");
#if defined(DEBUG)
    m_group_stats[idx].set_debug_is_free(false);
#endif // defined(DEBUG)

    return group;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
void task_scheduler::release_group(task_group group)
{
    XR_DEBUG_ASSERTION_MSG(!is_worker_thread(), "Can't use release_group inside Task.");
    XR_DEBUG_ASSERTION_MSG(group.is_valid(), "Invalid group ID");

    int idx = group.get_valid_index();
    XR_UNREFERENCED_PARAMETER(idx);
    XR_DEBUG_ASSERTION_MSG(m_group_stats[idx].get_debug_is_free() == false, "Group already released");
#if defined(DEBUG)
    m_group_stats[idx].set_debug_is_free(true);
#endif // defined(DEBUG)

    bool res = m_available_groups.enqueue(std::move(group));
    XR_UNREFERENCED_PARAMETER(res);
    XR_DEBUG_ASSERTION_MSG(res, "Can't return group to pool");
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
task_group_description& task_scheduler::get_group_desc(task_group group)
{
    XR_DEBUG_ASSERTION_MSG(group.is_valid(), "Invalid group ID");

    int idx = group.get_valid_index();
    task_group_description& group_desc = m_group_stats[idx];

    XR_DEBUG_ASSERTION_MSG(group_desc.get_debug_is_free() == false, "Invalid group");
    return group_desc;
}


#ifdef XR_INSTRUMENTED_BUILD
//-----------------------------------------------------------------------------------------------------------
/**
 */
void task_scheduler::notify_fibers_created(uint32_t fibersCount)
{
    if(base_profiler_event_listener* eventListener = get_profiler_event_listener())
        eventListener->on_fibers_created(fibersCount);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
void task_scheduler::notify_threads_created(uint32_t threadsCount)
{
    if(base_profiler_event_listener* eventListener = get_profiler_event_listener())
        eventListener->on_threads_created(threadsCount);
}
#endif

//-----------------------------------------------------------------------------------------------------------
/**
 */
size_t task_scheduler::effective_master_buckets(size_t tasks)
{
    XR_DEBUG_ASSERTION_MSG(tasks < (details::max_task_buffer_capacity - 1),
        "Too many tasks per one Run.");
    XR_DEBUG_ASSERTION_MSG(!is_worker_thread(),
        "Can't use run_async inside Task. Use fiber_context.run_async() instead.");

    uint32_t input_tasks = static_cast<uint32_t>(tasks);
    size_t bucket_count = eastl::min(get_workers_count(), input_tasks);
    return bucket_count;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
void task_scheduler::run_subtasks_on_scheduler(
    utils::array_view<details::task_bucket>& buckets,
    bool restored_from_awaiting)
{
    run_tasks_internal(buckets, nullptr, restored_from_awaiting);
}

XR_NAMESPACE_END(xr, tasks)
//-----------------------------------------------------------------------------------------------------------
