cmake_minimum_required(VERSION 3.10)

## Add modules function
include(${CMAKE_MODULE_PATH}/add_module.cmake)

if(${WITH_TESTS})
	include(${CMAKE_MODULE_PATH}/modules/include_catch.cmake)
endif(${WITH_TESTS})

include(CheckFunctionExists)
include(CheckIncludeFiles)
include(CMakeDependentOption)

project(corlib)

##

set(CORE_MODULE_HEADERS
	"include/corlib/types.h"
)

source_group("include" FILES ${CORE_MODULE_HEADERS})

##

set(CORE_MODULE_MACRO_HEADERS
	"include/corlib/macro/aligning.h"
	"include/corlib/macro/assertion.h"
	"include/corlib/macro/callconv.h"
	"include/corlib/macro/compiler.h"
	"include/corlib/macro/constexpr.h"
	"include/corlib/macro/interfacing.h"
	"include/corlib/macro/likely.h"
	"include/corlib/macro/namespaces.h"
	"include/corlib/macro/string.h"
	"include/corlib/macro/todo.h"
	"include/corlib/macro/unreferenced_parameter.h"
	"include/corlib/macro/variadic.h"
)

source_group("include\\macro" FILES ${CORE_MODULE_MACRO_HEADERS})

##

set(CORE_MODULE_MATH_HEADERS
	"include/corlib/math/color.h"
	"include/corlib/math/color_table.h"
	"include/corlib/math/constants.h"
	"include/corlib/math/cube.h"
	"include/corlib/math/local_transform.h"
	"include/corlib/math/mathlib.h"
	"include/corlib/math/matrix4.h"
	"include/corlib/math/quaternion.h"
	"include/corlib/math/random.h"
	"include/corlib/math/rect.h"
	"include/corlib/math/vector.h"
)

source_group("include\\math" FILES ${CORE_MODULE_MATH_HEADERS})

##

set(CORE_MODULE_MATH_SSE_HEADERS
	"include/corlib/math/details/sse/sse_bool_in_vec.h"
	"include/corlib/math/details/sse/sse_empty_guard.h"
	"include/corlib/math/details/sse/sse_float_in_vec.h"
	"include/corlib/math/details/sse/sse_math_intrinsics.h"
	"include/corlib/math/details/sse/sse_matrix4.h"
	"include/corlib/math/details/sse/sse_quaternion.h"
	"include/corlib/math/details/sse/sse_vec_idx_selector.h"
	"include/corlib/math/details/sse/sse_vector.h"
)

source_group("include\\math\\detais\\sse" FILES ${CORE_MODULE_MATH_SSE_HEADERS})

##

set(CORE_MODULE_MATH_SOURCES
	"sources/math/local_transform.cpp"
	"sources/math/matrix4.cpp"
	"sources/math/quaternion.cpp"
	"sources/math/random.cpp"
	"sources/math/vector.cpp"
)

source_group("sources\\math" FILES ${CORE_MODULE_MATH_SOURCES})

##

set(CORE_MODULE_MEMORY_HEADERS
	"include/corlib/memory/allocator_helper.h"
	"include/corlib/memory/allocator_macro.h"
	"include/corlib/memory/blob.h"
	"include/corlib/memory/buffer_range.h"
	"include/corlib/memory/buffer_ref.h"
	"include/corlib/memory/memory_aligned_allocator.h"
	"include/corlib/memory/memory_aligned_helpers.h"
	"include/corlib/memory/memory_allocator_base.h"
	"include/corlib/memory/memory_crt_allocator.h"
	"include/corlib/memory/memory_debug_parameters.h"
	"include/corlib/memory/memory_functions.h"
	"include/corlib/memory/memory_mt_arena_allocator.h"
	"include/corlib/memory/memory_paging.h"
	"include/corlib/memory/memory_st_arena_allocator.h"
	"include/corlib/memory/memory_static_allocator.h"
	"include/corlib/memory/memory_synchronized_allocator.h"
	"include/corlib/memory/profiler_event_listener.h"
	"include/corlib/memory/uninitialized_reference.h"
)

source_group("include\\memory" FILES ${CORE_MODULE_MEMORY_HEADERS})

##

set(CORE_MODULE_MEMORY_PROXY_HEADERS
	"include/corlib/memory/proxy/eastl_proxy_allocator.h"
)

source_group("include\\memory\\proxy" FILES ${CORE_MODULE_MEMORY_PROXY_HEADERS})

##

set(CORE_MODULE_MEMORY_SOURCES
	"sources/memory/blob.cpp"
	"sources/memory/memory_base_allocator.cpp"
	"sources/memory/memory_crt_allocator.cpp"
	"sources/memory/memory_functions.cpp"
	"sources/memory/memory_utility_for_arena.h")

source_group("sources\\memory" FILES ${CORE_MODULE_MEMORY_SOURCES})

##

set(CORE_MODULE_MEMORY_PROXY_SOURCES
	"sources/memory/proxy/eastl_proxy_allocator.cpp"
)

source_group("sources\\memory\\proxy" FILES ${CORE_MODULE_MEMORY_PROXY_SOURCES})

##

if(WIN32)
	set(CORE_MODULE_MEMORY_SOURCES_WIN32
		"sources/memory/memory_crt_allocator_win32.cpp"
		"sources/memory/memory_mt_arena_allocator_win32.cpp"
		"sources/memory/memory_paging_win32.cpp"
		"sources/memory/memory_st_arena_allocator_win32.cpp"
		"sources/memory/memory_utility_for_arena_win32.cpp"
		"sources/memory/memory_utility_for_arena_win32.h")
	
	source_group("sources\\memory" FILES ${CORE_MODULE_MEMORY_SOURCES_WIN32})
endif(WIN32)

##

set(CORE_MODULE_SYS_HEADERS
	"include/corlib/sys/arg_list.h"
	"include/corlib/sys/chrono.h"
	"include/corlib/sys/debug.h"
	"include/corlib/sys/dll.h"
	"include/corlib/sys/error.h"
	"include/corlib/sys/exit_handler.h"
	"include/corlib/sys/net.h"
	"include/corlib/sys/thread.h"
	"include/corlib/sys/tls.h")
	
source_group("include\\sys" FILES ${CORE_MODULE_SYS_HEADERS})

##

set(CORE_MODULE_SYS_SOURCES
	"sources/sys/arg_list.cpp"
	"sources/sys/exit_handler.cpp")
	
source_group("sources\\sys" FILES ${CORE_MODULE_SYS_SOURCES})

##

if(WIN32)
	set(CORE_MODULE_SYS_WIN_HEADERS
		"include/corlib/sys/win/com_wrapper.h"
		"include/corlib/sys/win/handle.h"
		"include/corlib/sys/win/min_windows.h")
	
	source_group("include\\sys\\win" FILES ${CORE_MODULE_SYS_WIN_HEADERS})
endif(WIN32)

##

if(WIN32)
	set(CORE_MODULE_SYS_WIN_SOURCES
		"sources/sys/win/handle_win32.cpp")
	
	source_group("sources\\sys\\win" FILES ${CORE_MODULE_SYS_WIN_SOURCES})
endif(WIN32)

##

if(WIN32)
	set(CORE_MODULE_SYS_SOURCES_WIN32
		"sources/sys/chrono_win32.cpp"
		"sources/sys/debug_win32.cpp"
		"sources/sys/dll_win32.cpp"
		"sources/sys/thread_win32.cpp"
		"sources/sys/tls_win32.cpp")
	
	source_group("sources\\sys" FILES ${CORE_MODULE_SYS_SOURCES_WIN32})
endif(WIN32)

##

if(WIN32)
	set(CORE_MODULE_SOURCES_WIN32
		"sources/error_conv_win32.cpp"
		"sources/error_conv_win32.h"
		"sources/os_include_win32.h")
	
	source_group("sources" FILES ${CORE_MODULE_SOURCES_WIN32})
endif(WIN32)

##

set(CORE_MODULE_TASK_HEADERS
	"include/corlib/tasks/profiler_event_listener.h"
	"include/corlib/tasks/task_aware_event.h"
	"include/corlib/tasks/task_aware_functions.h"
	"include/corlib/tasks/task_system.h"
)

source_group("include\\tasks" FILES ${CORE_MODULE_TASK_HEADERS})

##

set(CORE_MODULE_TASK_DETAILS_HEADERS
	"include/corlib/tasks/details/grouped_task.h"
	"include/corlib/tasks/details/task_bucket.h"
	"include/corlib/tasks/details/task_desc.h"
	"include/corlib/tasks/details/task_group.h"
	"include/corlib/tasks/details/work_distribution.h"
)

source_group("include\\tasks\\details" FILES ${CORE_MODULE_TASK_DETAILS_HEADERS})

##

set(CORE_MODULE_TASK_SOURCES
	"sources/tasks/allocator.h"
	"sources/tasks/concurrent_task_queue.h"
	"sources/tasks/fiber.cpp"
	"sources/tasks/fiber.h"
	"sources/tasks/fiber_context.cpp"
	"sources/tasks/fiber_context.h"
	"sources/tasks/mpmc_queue.h"
	"sources/tasks/scheduler.cpp"
	"sources/tasks/scheduler.h"
	"sources/tasks/task_system.cpp"
	"sources/tasks/thread_context.cpp"
	"sources/tasks/thread_context.h"
)

source_group("sources\\tasks" FILES ${CORE_MODULE_TASK_SOURCES})

##

set(CORE_MODULE_THREADING_HEADERS
	"include/corlib/threading/atomic_backoff.h"
	"include/corlib/threading/atomic_backoff_helpers.h"
	"include/corlib/threading/atomic_backoff_strategy.h"
	"include/corlib/threading/atomic_do_once.h"
	"include/corlib/threading/atomic_types.h"
	"include/corlib/threading/event.h"
	"include/corlib/threading/event_monitor.h"
	"include/corlib/threading/fast_semaphore.h"
	"include/corlib/threading/interlocked.h"
	"include/corlib/threading/lightweight_event.h"
	"include/corlib/threading/read_write_spin_wait.h"
	"include/corlib/threading/scoped_lock.h"
	"include/corlib/threading/scoped_managed_lock.h"
	"include/corlib/threading/sleeping_policy.h"
	"include/corlib/threading/spin_wait.h"
	"include/corlib/threading/spin_wait_strategy_traits.h"
	"include/corlib/threading/spin_wait_noop_locking_strategy.h"
	"include/corlib/threading/spin_wait_fairness_locking_strategy.h"
	"include/corlib/threading/spin_wait_speculative_locking_strategy.h"
	"include/corlib/threading/spin_wait_precise_locking_streategy.h"
	"include/corlib/threading/threading_policies.h"
	"include/corlib/threading/thread_traits.h")
	
source_group("include\\threading" FILES ${CORE_MODULE_THREADING_HEADERS})

##

set(CORE_MODULE_THREADING_SOURCES
	"sources/threading/atomic_do_once.cpp"
	"sources/threading/read_write_spin_wait.cpp")

source_group("sources\\threading" FILES ${CORE_MODULE_THREADING_SOURCES})

##

if(WIN32)
	set(CORE_MODULE_THREADING_SOURCES_WIN32
		"sources/threading/atomic_backoff_strategy_win32.cpp"
		"sources/threading/event_win32.cpp"
		"sources/threading/interlocked_tsx_extensions_win32.asm"
		"sources/threading/interlocked_win32.cpp"
		"sources/threading/lightweight_event_win32.cpp"
		"sources/threading/spin_wait_speculative_locking_strategy_win32.cpp"
		"sources/threading/spin_wait_precise_locking_streategy_win32.cpp")
	
	source_group("sources\\threading" FILES ${CORE_MODULE_THREADING_SOURCES_WIN32})
endif(WIN32)

##

set(CORE_MODULE_UTILS_HEADERS
	"include/corlib/utils/algorithm.h"
	"include/corlib/utils/aligning.h"
	"include/corlib/utils/array_view.h"
	"include/corlib/utils/buffer_string.h"
	"include/corlib/utils/constexpr_string.h"
	"include/corlib/utils/constexpr_map.h"
	"include/corlib/utils/constexpr_vector.h"
	"include/corlib/utils/check_invariants.h"
	"include/corlib/utils/delegate.h"
	"include/corlib/utils/deque.h"
	"include/corlib/utils/enumeration.h"
	"include/corlib/utils/fixed_string.h"
	"include/corlib/utils/fourcc.h"
	"include/corlib/utils/hash_crc32.h"
	"include/corlib/utils/hash_elsa.h"
	"include/corlib/utils/hash_string.h"
	"include/corlib/utils/integer.h"
	"include/corlib/utils/integer_fwd.h"
	"include/corlib/utils/integer_traits.h"
	"include/corlib/utils/intrusive_list_node.h"
	"include/corlib/utils/list.h"
	"include/corlib/utils/not_null_ptr.h"
	"include/corlib/utils/packed.h"
	"include/corlib/utils/padded.h"
	"include/corlib/utils/prehashed.h"
	"include/corlib/utils/ptr.h"
	"include/corlib/utils/pointer_cast.h"
	"include/corlib/utils/scoped.h"
	"include/corlib/utils/static_vector.h"
	"include/corlib/utils/string_conversion.h"
	"include/corlib/utils/string_map.h"
	"include/corlib/utils/string_view.h"
	"include/corlib/utils/select_sizet_constant.h"
	"include/corlib/utils/timer.h"
	"include/corlib/utils/type_conversions.h"
	"include/corlib/utils/type_inversions.h"
	"include/corlib/utils/type_traits.h"
	"include/corlib/utils/vector.h"
	"include/corlib/utils/unordered_map.h"
	"include/corlib/utils/unordered_set.h"
	"include/corlib/utils/utility.h"
)

source_group("include\\utils" FILES ${CORE_MODULE_UTILS_HEADERS})

##

set(CORE_MODULE_UTILS_SOURCES
	"sources/utils/string_conversion.cpp"
	"sources/utils/hash_crc32.cpp"
)

source_group("sources\\utils" FILES ${CORE_MODULE_UTILS_SOURCES})

##

set(CORE_MODULE_UTILS_DETAILS_HEADERS
	"include/corlib/utils/details/basic_buffer_string.h"
	"include/corlib/utils/details/basic_constexpr_string.h"
	"include/corlib/utils/details/char_traits.h"
	"include/corlib/utils/details/compare_key.h"
	"include/corlib/utils/details/constexpr_knuth_morris_pratt_searcher.h"
	"include/corlib/utils/details/constexpr_boyer_moore_searcher.h"
	"include/corlib/utils/details/constexpr_binarysearch.h"
	"include/corlib/utils/details/constexpr_lowerbound.h"
	"include/corlib/utils/details/constexpr_partition.h"
	"include/corlib/utils/details/constexpr_quicksort.h"
	"include/corlib/utils/details/constexpr_swap.h"
)

source_group("include\\utils\\details" FILES ${CORE_MODULE_UTILS_DETAILS_HEADERS})

##

set(SOURCES
	${CORE_MODULE_HEADERS}
	${CORE_MODULE_MACRO_HEADERS}
	${CORE_MODULE_MATH_HEADERS}
	${CORE_MODULE_MATH_SSE_HEADERS}
	${CORE_MODULE_MATH_SOURCES}
	${CORE_MODULE_MEMORY_SOURCES}
	${CORE_MODULE_MEMORY_HEADERS}
	${CORE_MODULE_MEMORY_PROXY_SOURCES}
	${CORE_MODULE_MEMORY_PROXY_HEADERS}
	${CORE_MODULE_SYS_SOURCES}
	${CORE_MODULE_SYS_HEADERS}
	${CORE_MODULE_TASK_HEADERS}
	${CORE_MODULE_TASK_DETAILS_HEADERS}
	${CORE_MODULE_TASK_SOURCES}
	${CORE_MODULE_THREADING_SOURCES}
	${CORE_MODULE_THREADING_HEADERS}
	${CORE_MODULE_UTILS_HEADERS}
	${CORE_MODULE_UTILS_SOURCES}
	${CORE_MODULE_UTILS_DETAILS_HEADERS}
)

if(WIN32)
	list(APPEND SOURCES ${CORE_MODULE_SOURCES_WIN32})
	list(APPEND SOURCES ${CORE_MODULE_MEMORY_SOURCES_WIN32})
	list(APPEND SOURCES ${CORE_MODULE_SYS_SOURCES_WIN32})
	list(APPEND SOURCES ${CORE_MODULE_SYS_WIN_HEADERS})
	list(APPEND SOURCES ${CORE_MODULE_SYS_WIN_SOURCES})
	list(APPEND SOURCES ${CORE_MODULE_THREADING_SOURCES_WIN32})
endif(WIN32)

##

set(OPTIONS generic:cpp17=yes generic:noexceptions=yes win:asm=yes)
set(DEPENDENCY module:eastl)

xrng_engine_add_module(${PROJECT_NAME} STATIC OPTIONS DEPENDENCY SOURCES)

## For Visual Studio
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER ${XR_PROJECT_PREFIX})

####################### TESTS #######################

if(${WITH_TESTS})

##

set(CORE_MODULE_NON_FILTERED_TESTS
	"tests/notnull_tests.cpp"
	"tests/ptr_tests.cpp")

source_group("\\" FILES ${CORE_MODULE_NON_FILTERED_TESTS})

##

set(CORE_MODULE_MATH_TESTS
	"tests/math/sse_vector_tests.cpp"
)

source_group("math" FILES ${CORE_MODULE_MATH_TESTS})

##

set(CORE_MODULE_MEMORY_TESTS
	"tests/memory/memory_aligned_allocator_tests.cpp"
	"tests/memory/memory_crt_allocator_tests.cpp"
	"tests/memory/memory_fixed_size_allocator_tests.cpp"
	"tests/memory/memory_functions_tests.cpp"
	"tests/memory/memory_mt_arena_allocator_tests.cpp"
	"tests/memory/memory_st_arena_allocator_tests.cpp")

source_group("memory" FILES ${CORE_MODULE_MEMORY_TESTS})

##

#set(CORE_MODULE_SYS_TESTS
#	"tests/sys/arg_list_tests.cpp"
#	"tests/sys/thread_tests.cpp")

#source_group("sys" FILES ${CORE_MODULE_SYS_TESTS})

##

set(CORE_MODULE_TASKS_TESTS
	"tests/tasks/fiber_tests.cpp"
	"tests/tasks/task_allocator_tests.cpp"
	"tests/tasks/task_tests.cpp"
)

source_group("tasks" FILES ${CORE_MODULE_TASKS_TESTS})

##

#set(CORE_MODULE_THREADING_TESTS 
#	"tests/threading/interlocked_tests.cpp")

#source_group("sys" FILES ${CORE_MODULE_THREADING_TESTS})

##

set(CORE_MODULE_TESTS "tests/unittests.cpp")
source_group("\\" FILES ${CORE_MODULE_TESTS})

##

set(TESTS
	${CORE_MODULE_TESTS}
	${CORE_MODULE_NON_FILTERED_TESTS}
	${CORE_MODULE_MATH_TESTS}
	${CORE_MODULE_MEMORY_TESTS}
	${CORE_MODULE_SYS_TESTS}
	${CORE_MODULE_TASKS_TESTS}
	${CORE_MODULE_THREADPOOL_TESTS}
	${CORE_MODULE_THREADING_TESTS})

set(TESTS_DEPENDENCY ${DEPENDENCY} module:${PROJECT_NAME})
set(TESTS_OPTIONS generic:cpp17=yes win:asm=yes)

xrng_engine_add_unittest(${PROJECT_NAME}_tests TESTS_OPTIONS TESTS_DEPENDENCY TESTS)
xrng_engine_add_doctest(${PROJECT_NAME}_tests)

## For Visual Studio
set_target_properties(${PROJECT_NAME}_tests PROPERTIES FOLDER ${XR_PROJECT_PREFIX})

endif(${WITH_TESTS})