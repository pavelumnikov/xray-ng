// This file is a part of xray-ng engine
//

#include "thread_context.h"
#include "corlib/memory/allocator_macro.h"

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, tasks, details)

//-----------------------------------------------------------------------------------------------------------
/**
 */
thread_context::thread_context(memory::base_allocator& alloc)
    : allocator { &alloc }
    , last_active_fiber_context { nullptr }
    , current_scheduler { nullptr }
    , current_thread {}
    , current_thread_id { 0 }
    , scheduler_fiber {}
    , queue {}
    , has_new_tasks_event {}
    , state { (uint32_t)thread_state::ALIVE }
    , desc_buffer { nullptr }
    , current_worker_index { 0 }
    , is_external_desc_buffer { false }
{
    XR_DEBUG_ASSERTION_MSG(allocator != nullptr,
        "Memory allocator must be initialized before thread_context access");

    queue.initialize_memory_pool(*allocator);

    desc_buffer = XR_ALLOCATE_MEMORY(*allocator, 
        memory_requrements_for_desc_buffer, "thread_context local data");
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
thread_context::thread_context(void* externalDescBuffer)
    : last_active_fiber_context { nullptr }
    , current_scheduler { nullptr }
    , current_thread {}
    , current_thread_id { 0 }
    , scheduler_fiber {}
    , queue {}
    , has_new_tasks_event {}
    , state { (uint32_t)thread_state::ALIVE }
    , desc_buffer { nullptr }
    , current_worker_index { 0 }
    , is_external_desc_buffer { true }
{
    desc_buffer = externalDescBuffer;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
thread_context::~thread_context()
{
    if(is_external_desc_buffer == false)
    {
        XR_DEBUG_ASSERTION_MSG(allocator != nullptr,
            "Memory allocator must be initialized before thread_context access");
        queue.shutdown_memory_pool(*allocator);
        XR_DEALLOCATE_MEMORY(*allocator, desc_buffer);
    }
    desc_buffer = nullptr;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
void thread_context::set_thread_index(uint32_t threadIndex)
{
    current_worker_index = threadIndex;
}

#ifdef XR_INSTRUMENTED_BUILD

//-----------------------------------------------------------------------------------------------------------
/**
 */
void thread_context::notify_wait_started()
{
    if(base_profiler_event_listener* event_listener = current_scheduler->get_profiler_event_listener())
    {
        event_listener->on_thread_wait_started();
    }
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
void thread_context::notify_wait_finished()
{
    if(base_profiler_event_listener* event_listener = current_scheduler->get_profiler_event_listener())
    {
        event_listener->on_thread_wait_finished();
    }
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
void thread_context::notify_temporary_worker_thread_join()
{
    if(base_profiler_event_listener* event_listener = current_scheduler->get_profiler_event_listener())
    {
        event_listener->on_temporary_worker_thread_join();
    }
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
void thread_context::notify_temporary_worker_thread_leave()
{
    if(base_profiler_event_listener* event_listener = current_scheduler->get_profiler_event_listener())
    {
        event_listener->on_temporary_worker_thread_leave();
    }
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
void thread_context::notify_task_execute_state_changed(math::color_table debug_color, const mt_char* debug_id, task_execute_state type, int32 fiber_index)
{
    if(base_profiler_event_listener* event_listener = current_scheduler->get_profiler_event_listener())
    {
        event_listener->on_task_execute_state_changed(debug_color, debug_id, type, fiber_index);
    }
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
void thread_context::notify_thread_created(uint32_t threadIndex)
{
    if(base_profiler_event_listener* event_listener = current_scheduler->get_profiler_event_listener())
    {
        event_listener->on_thread_created(threadIndex);
    }
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
void thread_context::notify_thread_started(uint32_t threadIndex)
{
    if(base_profiler_event_listener* event_listener = current_scheduler->get_profiler_event_listener())
    {
        event_listener->on_thread_started(threadIndex);
    }
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
void thread_context::notify_thread_stopped(uint32_t threadIndex)
{
    if(base_profiler_event_listener* event_listener = current_scheduler->get_profiler_event_listener())
    {
        event_listener->on_thread_stoped(threadIndex);
    }
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
void thread_context::notify_thread_idle_started(uint32_t threadIndex)
{
    if(base_profiler_event_listener* event_listener = current_scheduler->get_profiler_event_listener())
    {
        event_listener->on_thread_idle_started(threadIndex);
    }
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
void thread_context::notify_thread_idle_finished(uint32_t threadIndex)
{
    if(base_profiler_event_listener* event_listener = current_scheduler->get_profiler_event_listener())
    {
        event_listener->on_thread_idle_finished(threadIndex);
    }
}

#endif // XR_INSTRUMENTED_BUILD

XR_NAMESPACE_END(xr, tasks, details)
//-----------------------------------------------------------------------------------------------------------
