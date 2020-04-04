cmake_minimum_required(VERSION 3.10)

## Add modules function
include(${CMAKE_MODULE_PATH}/add_module.cmake)

if(${WITH_TESTS})
	include(${CMAKE_MODULE_PATH}/modules/include_catch.cmake)
endif(${WITH_TESTS})

include(CheckFunctionExists)
include(CheckIncludeFiles)
include(CMakeDependentOption)

project(eastl)

##

set(EASTL_SDK_HEADERS
	"include/EASTL/algorithm.h"
	"include/EASTL/allocator.h"
	"include/EASTL/any.h"
	"include/EASTL/array.h"
	"include/EASTL/bitset.h"
	"include/EASTL/bitvector.h"
	"include/EASTL/chrono.h"
	"include/EASTL/core_allocator.h"
	"include/EASTL/core_allocator_adapter.h"
	"include/EASTL/deque.h"
	"include/EASTL/fixed_allocator.h"
	"include/EASTL/fixed_function.h"
	"include/EASTL/fixed_hash_map.h"
	"include/EASTL/fixed_hash_set.h"
	"include/EASTL/fixed_list.h"
	"include/EASTL/fixed_map.h"
	"include/EASTL/fixed_set.h"
	"include/EASTL/fixed_slist.h"
	"include/EASTL/fixed_string.h"
	"include/EASTL/fixed_substring.h"
	"include/EASTL/fixed_vector.h"
	"include/EASTL/functional.h"
	"include/EASTL/hash_map.h"
	"include/EASTL/hash_set.h"
	"include/EASTL/heap.h"
	"include/EASTL/initializer_list.h"
	"include/EASTL/intrusive_hash_map.h"
	"include/EASTL/intrusive_hash_set.h"
	"include/EASTL/intrusive_list.h"
	"include/EASTL/intrusive_ptr.h"
	"include/EASTL/iterator.h"
	"include/EASTL/linked_array.h"
	"include/EASTL/linked_ptr.h"
	"include/EASTL/list.h"
	"include/EASTL/map.h"
	"include/EASTL/memory.h"
	"include/EASTL/meta.h"
	"include/EASTL/numeric.h"
	"include/EASTL/numeric_limits.h"
	"include/EASTL/optional.h"
	"include/EASTL/priority_queue.h"
	"include/EASTL/queue.h"
	"include/EASTL/random.h"
	"include/EASTL/ratio.h"
	"include/EASTL/safe_ptr.h"
	"include/EASTL/scoped_array.h"
	"include/EASTL/scoped_ptr.h"
	"include/EASTL/segmented_vector.h"
	"include/EASTL/set.h"
	"include/EASTL/shared_array.h"
	"include/EASTL/shared_ptr.h"
	"include/EASTL/slist.h"
	"include/EASTL/sort.h"
	"include/EASTL/span.h"
	"include/EASTL/stack.h"
	"include/EASTL/string.h"
	"include/EASTL/string_hash_map.h"
	"include/EASTL/string_map.h"
	"include/EASTL/string_view.h"
	"include/EASTL/tuple.h"
	"include/EASTL/type_traits.h"
	"include/EASTL/unique_ptr.h"
	"include/EASTL/unordered_map.h"
	"include/EASTL/unordered_set.h"
	"include/EASTL/utility.h"
	"include/EASTL/variant.h"
	"include/EASTL/vector.h"
	"include/EASTL/vector_map.h"
	"include/EASTL/vector_multimap.h"
	"include/EASTL/vector_multiset.h"
	"include/EASTL/vector_set.h"
	"include/EASTL/version.h"
	"include/EASTL/weak_ptr.h")
	
source_group("include" FILES ${EASTL_SDK_HEADERS})

##

set(EASTL_SDK_SOURCES
	"sources/assert.cpp"
	"sources/fixed_pool.cpp"
	"sources/hashtable.cpp"
	"sources/intrusive_list.cpp"
	"sources/numeric_limits.cpp"
	"sources/red_black_tree.cpp"
	"sources/string.cpp"
	"sources/thread_support.cpp")
	
source_group("sources" FILES ${EASTL_SDK_SOURCES})

##

set(EASTL_SDK_EASTDC_SOURCES
	"sources/EAStdC/EAMemory.cpp"
	"sources/EAStdC/EASprintf.cpp")
	
source_group("sources\\EAStdC" FILES ${EASTL_SDK_EASTDC_SOURCES})

##

set(EASTL_SDK_BONUS_HEADERS
	"include/EASTL/bonus/adaptors.h"
	"include/EASTL/bonus/call_traits.h"
	"include/EASTL/bonus/compressed_pair.h"
	"include/EASTL/bonus/fixed_ring_buffer.h"
	"include/EASTL/bonus/fixed_tuple_vector.h"
	"include/EASTL/bonus/intrusive_sdlist.h"
	"include/EASTL/bonus/intrusive_slist.h"
	"include/EASTL/bonus/list_map.h"
	"include/EASTL/bonus/ring_buffer.h"
	"include/EASTL/bonus/sort_extra.h"
	"include/EASTL/bonus/sparse_matrix.h"
	"include/EASTL/bonus/tuple_vector.h")

source_group("include\\bonus" FILES ${EASTL_SDK_BONUS_HEADERS})

##

set(EASTL_SDK_EABASE_HEADERS
	"include/EASTL/EABase/eabase.h"
	"include/EASTL/EABase/eahave.h"
	"include/EASTL/EABase/earesult.h"
	"include/EASTL/EABase/eastdarg.h"
	"include/EASTL/EABase/eaunits.h"
	"include/EASTL/EABase/int128.h"
	"include/EASTL/EABase/nullptr.h"
	"include/EASTL/EABase/version.h")
	
source_group("include\\EABase" FILES ${EASTL_SDK_EABASE_HEADERS})

##

set(EASTL_SDK_EABASE_CONFIG_HEADERS
	"include/EASTL/EABase/config/eacompiler.h"
	"include/EASTL/EABase/config/eacompilertraits.h"
	"include/EASTL/EABase/config/eaplatform.h")
	
source_group("include\\EABase\\config" FILES ${EASTL_SDK_EABASE_CONFIG_HEADERS})

##

set(EASTL_SDK_EASTDC_HEADERS
	"include/EASTL/EAStdC/EAAlignment.h"
	"include/EASTL/EAStdC/EADateTime.h"
	"include/EASTL/EAStdC/EAMemory.h"
	"include/EASTL/EAStdC/EAProcess.h"
	"include/EASTL/EAStdC/EASprintf.h"
	"include/EASTL/EAStdC/EAStopWatch.h"
	"include/EASTL/EAStdC/EAString.h"
	"include/EASTL/EAStdC/EATextUtil.h")
	
source_group("include\\EAStdC" FILES ${EASTL_SDK_EASTDC_HEADERS})

##

set(EASTL_SDK_INTERNAL_HEADERS
	"include/EASTL/internal/allocator_traits.h"
	"include/EASTL/internal/allocator_traits_fwd_decls.h"
	"include/EASTL/internal/char_traits.h"
	"include/EASTL/internal/config.h"
	"include/EASTL/internal/copy_help.h"
	"include/EASTL/internal/enable_shared.h"
	"include/EASTL/internal/fill_help.h"
	"include/EASTL/internal/fixed_pool.h"
	"include/EASTL/internal/function.h"
	"include/EASTL/internal/function_detail.h"
	"include/EASTL/internal/function_help.h"
	"include/EASTL/internal/functional_base.h"
	"include/EASTL/internal/generic_iterator.h"
	"include/EASTL/internal/hashtable.h"
	"include/EASTL/internal/in_place_t.h"
	"include/EASTL/internal/integer_sequence.h"
	"include/EASTL/internal/intrusive_hashtable.h"
	"include/EASTL/internal/mem_fn.h"
	"include/EASTL/internal/memory_base.h"
	"include/EASTL/internal/move_help.h"
	"include/EASTL/internal/pair_fwd_decls.h"
	"include/EASTL/internal/piecewise_construct_t.h"
	"include/EASTL/internal/red_black_tree.h"
	"include/EASTL/internal/smart_ptr.h"
	"include/EASTL/internal/thread_support.h"
	"include/EASTL/internal/tuple_fwd_decls.h"
	"include/EASTL/internal/type_compound.h"
	"include/EASTL/internal/type_fundamental.h"
	"include/EASTL/internal/type_pod.h"
	"include/EASTL/internal/type_properties.h"
	"include/EASTL/internal/type_transformations.h")
	
source_group("include\\internal" FILES ${EASTL_SDK_INTERNAL_HEADERS})

##

set(SOURCES
	${EASTL_SDK_SOURCES}
	${EASTL_SDK_HEADERS}
	${EASTL_SDK_BONUS_HEADERS}
	${EASTL_SDK_EABASE_HEADERS}
	${EASTL_SDK_EABASE_CONFIG_HEADERS}
	${EASTL_SDK_EASTDC_SOURCES}
	${EASTL_SDK_EASTDC_HEADERS}
	${EASTL_SDK_INTERNAL_HEADERS})

##

set(OPTIONS generic:cpp17=yes generic:noexceptions=yes)
xrng_engine_add_thirdparty(${PROJECT_NAME} STATIC OPTIONS NoDependency SOURCES)

target_compile_definitions(${PROJECT_NAME} PUBLIC _CHAR16T)
target_compile_definitions(${PROJECT_NAME} PUBLIC EASTL_OPENSOURCE=1)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

## For Visual Studio
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER ${XR_PROJECT_PREFIX})

####################### TESTS #######################

if(${WITH_TESTS})

##

set(EASTL_SDK_TESTS
	"tests/ConceptImpls.h"
	"tests/EASTLTest.cpp"
	"tests/EASTLTest.h"
	"tests/EASTLTestAllocator.cpp"
	"tests/EASTLTestAllocator.h"
	"tests/GetTypeName.h"
	"tests/main.cpp"
	"tests/TestAlgorithm.cpp"
	"tests/TestAllocator.cpp"
	##"tests/TestAny.cpp"
	##"tests/TestArray.cpp"
	##"tests/TestBitset.cpp"
	##"tests/TestBitVector.cpp"
	##"tests/TestCharTraits.cpp"
	##"tests/TestChrono.cpp"
	##"tests/TestCppCXTypeTraits.cpp"
	##"tests/TestDeque.cpp"
	##"tests/TestExtra.cpp"
	##"tests/TestFinally.cpp"
	##"tests/TestFixedFunction.cpp"
	##"tests/TestFixedHash.cpp"
	##"tests/TestFixedList.cpp"
	##"tests/TestFixedMap.cpp"
	##"tests/TestFixedSet.cpp"
	##"tests/TestFixedSList.cpp"
	##"tests/TestFixedString.cpp"
	##"tests/TestFixedTupleVector.cpp"
	##"tests/TestFixedVector.cpp"
	##"tests/TestFunctional.cpp"
	##"tests/TestHash.cpp"
	##"tests/TestHeap.cpp"
	##"tests/TestIntrusiveHash.cpp"
	##"tests/TestIntrusiveList.cpp"
	##"tests/TestIntrusiveSDList.cpp"
	##"tests/TestIntrusiveSList.cpp"
	##"tests/TestIterator.cpp"
	##"tests/TestList.cpp"
	##"tests/TestListMap.cpp"
	##"tests/TestLruCache.cpp"
	##"tests/TestMap.cpp"
	##"tests/TestMap.h"
	##"tests/TestMemory.cpp"
	##"tests/TestMeta.cpp"
	##"tests/TestNumericLimits.cpp"
	##"tests/TestOptional.cpp"
	##"tests/TestRandom.cpp"
	##"tests/TestRatio.cpp"
	##"tests/TestRingBuffer.cpp"
	##"tests/TestSegmentedVector.cpp"
	##"tests/TestSet.cpp"
	##"tests/TestSet.h"
	##"tests/TestSList.cpp"
	##"tests/TestSmartPtr.cpp"
	##"tests/TestSort.cpp"
	##"tests/TestSpan.cpp"
	##"tests/TestSparseMatrix.cpp"
	##"tests/TestString.cpp"
	##"tests/TestString.inl"
	##"tests/TestStringHashMap.cpp"
	##"tests/TestStringMap.cpp"
	##"tests/TestStringView.cpp"
	##"tests/TestStringView.inl"
	##"tests/TestTuple.cpp"
	##"tests/TestTupleVector.cpp"
	##"tests/TestTypeTraits.cpp"
	##"tests/TestUtility.cpp"
	##"tests/TestVariant.cpp"
	##"tests/TestVector.cpp"
	##"tests/TestVectorMap.cpp"
	##"tests/TestVectorSet.cpp"
)

source_group("\\" FILES ${EASTL_SDK_TESTS})

##

set(TESTS_DEPENDENCY ${DEPENDENCY} module:${PROJECT_NAME})
set(TESTS_OPTIONS generic:cpp17=yes win:asm=yes)

xrng_engine_add_unittest(${PROJECT_NAME}_tests TESTS_OPTIONS TESTS_DEPENDENCY EASTL_SDK_TESTS)
xrng_engine_add_doctest(${PROJECT_NAME}_tests)

## For Visual Studio
set_target_properties(${PROJECT_NAME}_tests PROPERTIES FOLDER ${XR_PROJECT_PREFIX})

endif(${WITH_TESTS})
