// This file is a part of xray-ng engine
//

#pragma once

//#include "wait_free_queue.h"
#include "fiber.h"
//#include "mpmc_queue.h"
#include "concurrent_task_queue.h"
#include "corlib/tasks/details/grouped_task.h"
#include "corlib/sys/thread.h"
#include "corlib/threading/event.h"
#include "corlib/math/random.h"


#ifdef XR_INSTRUMENTED_BUILD

#define XR_SYSTEM_TASK_COLOR (xr::tasks::Color::Yellow)
#define XR_SYSTEM_TASK_NAME "SchedulerTask"
#define MT_SYSTEM_TASK_FIBER_NAME "IdleFiber"
#define XR_SYSTEM_FIBER_INDEX (int32)(-1)

#endif


//------------------------------------------------------------------------------
namespace xr::tasks
{

// forward declarations
class fiber_context;
class task_scheduler;

#ifdef XR_INSTRUMENTED_BUILD
enum class task_execute_state
{
    start,
    stop,
    resume,
    suspend
}; // enum class task_execute_state
#endif

} // namespace xr::tasks
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
namespace xr::tasks::details
{

constexpr size_t max_priority_count = 3;
constexpr size_t max_task_buffer_capacity = 4096;

using task_queue = priority_task_queue<
    grouped_task, max_priority_count, max_task_buffer_capacity>;

constexpr size_t memory_requrements_for_desc_buffer = 
    sizeof(grouped_task) * max_task_buffer_capacity;

//------------------------------------------------------------------------------
enum class thread_state
{
    ALIVE = 0,
    EXIT = 1
};

//------------------------------------------------------------------------------
struct thread_context
{
    memory::base_allocator* allocator;

    fiber_context* last_active_fiber_context;

    // pointer to task manager
    task_scheduler* current_scheduler;

    // thread
    sys::thread_handle current_thread;

    // thread Id
    sys::thread_id current_thread_id;

    // scheduler fiber
    fiber scheduler_fiber;

    // task queue awaiting execution
    task_queue queue;

    // new task has arrived to queue event
    threading::event has_new_tasks_event;

    // thread is alive or not
    volatile uint32_t state;

    // Temporary buffer, fixed size = max_task_buffer_capacity
    void* desc_buffer;

    // Thread index
    uint32_t current_worker_index;

    // Task stream lane selector for local tasks
    uint32_t current_lane_index { 0 };

    // Thread random number generator
    math::fast_random<uint16_t> random { rand() };

    bool is_external_desc_buffer;

    // prevent false cache sharing between threads
    uint8_t cacheline[64];

    thread_context(memory::base_allocator& alloc);
    thread_context(void* external_buffer);
    ~thread_context();

    void set_thread_index(uint32_t thread_index);

#ifdef XR_INSTRUMENTED_BUILD

    void notify_thread_created(uint32_t thread_index);
    void notify_thread_started(uint32_t thread_index);
    void notify_thread_stopped(uint32_t thread_index);

    void notify_task_execute_state_changed(math::color_table debug_color, pcstr debug_id,
        task_execute_state type, uint32_t fiber_index);

    void notify_thread_idle_started(uint32_t thread_index);
    void notify_thread_idle_finished(uint32_t thread_index);

    void notify_wait_started();
    void notify_wait_finished();

    void notify_temporary_worker_thread_join();
    void notify_temporary_worker_thread_leave();

#endif

}; // struct thread_context

} // namespace xr::tasks::details
//------------------------------------------------------------------------------
