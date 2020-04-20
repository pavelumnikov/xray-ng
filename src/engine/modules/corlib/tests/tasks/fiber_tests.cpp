// This file is a part of xray-ng engine
//

#include "catch/catch.hpp"
#include "../sources/tasks/fiber.h"
#include "corlib/threading/interlocked.h"
#include "corlib/types.h"

using namespace xr;

constexpr size_t commit_size = 64_kb;
constexpr size_t reserve_size = 512_kb;

struct single_fiber_arg
{
    threading::atomic_uint32 counter { 0 };
    tasks::fiber main_fiber;
    tasks::fiber other_fiber;
};

void single_fiber_start(void* arg)
{
    auto singleFiberArg = reinterpret_cast<single_fiber_arg*>(arg);

    threading::atomic_fetch_add_seq(singleFiberArg->counter, 1U);
    tasks::fiber::switch_to(singleFiberArg->other_fiber, singleFiberArg->main_fiber);

    // We should never get here
    FAIL();
}

void main_fiber_start(void* arg)
{
    auto mainFiberArg = reinterpret_cast<single_fiber_arg*>(arg);
    tasks::fiber::switch_to(mainFiberArg->main_fiber, mainFiberArg->other_fiber);
}

TEST_CASE("Single Fiber Switch", "[fiber]")
{
    single_fiber_arg arg;
    arg.counter = 0;
    arg.other_fiber.create(commit_size, reserve_size, single_fiber_start, &arg);
    arg.main_fiber.create_from_thread_and_run(main_fiber_start, &arg);

    REQUIRE(arg.counter == 1);
}

//-----------------------------------------------------------------------------------------------------------

struct multiple_fiber_arg
{
    volatile uint64_t counter { 0 };
    tasks::fiber main_fiber;
    tasks::fiber first_fiber;
    tasks::fiber second_fiber;
    tasks::fiber third_fiber;
    tasks::fiber fourth_fiber;
    tasks::fiber fifth_fiber;
    tasks::fiber sixth_fiber;
};

void first_level_fiber_start(void* arg)
{
    auto* singleFiberArg = reinterpret_cast<multiple_fiber_arg*>(arg);

    singleFiberArg->counter += 8;
    tasks::fiber::switch_to(singleFiberArg->first_fiber, singleFiberArg->second_fiber);

    // Return from sixth
    // We just finished 1 -> 2 -> 3 -> 4 -> 5 -> 6 -> 1
    // Let's do an intermediate check
    REQUIRE(((((((0ULL + 8ULL) * 3ULL) + 7ULL) * 6ULL) - 9ULL) * 2ULL) == singleFiberArg->counter);

    // Now run the rest of the sequence
    singleFiberArg->counter *= 4;
    tasks::fiber::switch_to(singleFiberArg->first_fiber, singleFiberArg->fifth_fiber);

    // Return from fifth
    singleFiberArg->counter += 1;
    tasks::fiber::switch_to(singleFiberArg->first_fiber, singleFiberArg->third_fiber);

    // We should never get here
    FAIL();
}

void second_level_fiber_start(void* arg)
{
    auto* singleFiberArg = reinterpret_cast<multiple_fiber_arg*>(arg);

    singleFiberArg->counter *= 3;
    tasks::fiber::switch_to(singleFiberArg->second_fiber, singleFiberArg->third_fiber);

    // Return from third
    singleFiberArg->counter += 9;
    tasks::fiber::switch_to(singleFiberArg->second_fiber, singleFiberArg->fourth_fiber);

    // Return from fourth
    singleFiberArg->counter += 7;
    tasks::fiber::switch_to(singleFiberArg->second_fiber, singleFiberArg->fifth_fiber);

    // We should never get here
    FAIL();
}

void third_level_fiber_start(void* arg)
{
    auto* singleFiberArg = reinterpret_cast<multiple_fiber_arg*>(arg);

    singleFiberArg->counter += 7;
    tasks::fiber::switch_to(singleFiberArg->third_fiber, singleFiberArg->fourth_fiber);

    // Return from first
    singleFiberArg->counter *= 3;
    tasks::fiber::switch_to(singleFiberArg->third_fiber, singleFiberArg->second_fiber);

    // Return from fifth
    singleFiberArg->counter *= 6;
    tasks::fiber::switch_to(singleFiberArg->third_fiber, singleFiberArg->sixth_fiber);

    // We should never get here
    FAIL();
}

void fourth_level_fiber_start(void* arg)
{
    auto* singleFiberArg = reinterpret_cast<multiple_fiber_arg*>(arg);

    singleFiberArg->counter *= 6;
    tasks::fiber::switch_to(singleFiberArg->fourth_fiber, singleFiberArg->fifth_fiber);

    // Return from second
    singleFiberArg->counter += 8;
    tasks::fiber::switch_to(singleFiberArg->fourth_fiber, singleFiberArg->sixth_fiber);

    // Return from sixth
    singleFiberArg->counter *= 5;
    tasks::fiber::switch_to(singleFiberArg->fourth_fiber, singleFiberArg->second_fiber);

    // We should never get here
    FAIL();
}

void fifth_level_fiber_start(void* arg)
{
    auto* singleFiberArg = reinterpret_cast<multiple_fiber_arg*>(arg);

    singleFiberArg->counter -= 9;
    tasks::fiber::switch_to(singleFiberArg->fifth_fiber, singleFiberArg->sixth_fiber);

    // Return from first
    singleFiberArg->counter *= 5;
    tasks::fiber::switch_to(singleFiberArg->fifth_fiber, singleFiberArg->first_fiber);

    // Return from second
    singleFiberArg->counter += 1;
    tasks::fiber::switch_to(singleFiberArg->fifth_fiber, singleFiberArg->third_fiber);

    // We should never get here
    FAIL();
}

void sixth_level_fiber_start(void* arg)
{
    auto* singleFiberArg = reinterpret_cast<multiple_fiber_arg*>(arg);

    singleFiberArg->counter *= 2;
    tasks::fiber::switch_to(singleFiberArg->sixth_fiber, singleFiberArg->first_fiber);

    // Return from fourth
    singleFiberArg->counter -= 9;
    tasks::fiber::switch_to(singleFiberArg->sixth_fiber, singleFiberArg->fourth_fiber);

    // Return from third
    singleFiberArg->counter -= 3;
    tasks::fiber::switch_to(singleFiberArg->sixth_fiber, singleFiberArg->main_fiber);

    // We should never get here
    FAIL();
}

void main_level_fiber_state(void* arg)
{
    auto* singleFiberArg = reinterpret_cast<multiple_fiber_arg*>(arg);
    tasks::fiber::switch_to(singleFiberArg->main_fiber, singleFiberArg->first_fiber);
}


TEST_CASE("Nested Fiber Switch", "[fiber]")
{
    multiple_fiber_arg arg;
    arg.counter = 0ULL;
    arg.first_fiber.create(commit_size, reserve_size, first_level_fiber_start, &arg);
    arg.second_fiber.create(commit_size, reserve_size, second_level_fiber_start, &arg);
    arg.third_fiber.create(commit_size, reserve_size, third_level_fiber_start, &arg);
    arg.fourth_fiber.create(commit_size, reserve_size, fourth_level_fiber_start, &arg);
    arg.fifth_fiber.create(commit_size, reserve_size, fifth_level_fiber_start, &arg);
    arg.sixth_fiber.create(commit_size, reserve_size, sixth_level_fiber_start, &arg);
    arg.main_fiber.create_from_thread_and_run(main_level_fiber_state, &arg);

    // The order should be:
    // Main -> 1 -> 2 -> 3 -> 4 -> 5 -> 6 -> 1 -> 5 -> 1 -> 3 -> 2 -> 4 -> 6 -> 4 -> 2 -> 5 -> 3 -> 6 -> Main

    REQUIRE(((((((((((((((((((0ULL + 8ULL) * 3ULL) + 7ULL) * 6ULL) - 9ULL) * 2ULL) * 4) * 5) + 1) * 3) + 9) + 8) - 9) * 5) + 7) + 1) * 6) - 3) == arg.counter);
}

//-----------------------------------------------------------------------------------------------------------

struct fp_multiple_fiber_arg
{
    volatile double counter { 0.0 };
    tasks::fiber main_fiber;
    tasks::fiber first_fiber;
    tasks::fiber second_fiber;
    tasks::fiber third_fiber;
    tasks::fiber fourth_fiber;
    tasks::fiber fifth_fiber;
    tasks::fiber sixth_fiber;
};

void fp_first_level_fiber_start(void* arg)
{
    auto* singleFiberArg = reinterpret_cast<fp_multiple_fiber_arg*>(arg);

    singleFiberArg->counter += 8.0;
    tasks::fiber::switch_to(singleFiberArg->first_fiber, singleFiberArg->second_fiber);

    // Return from sixth
    // We just finished 1 -> 2 -> 3 -> 4 -> 5 -> 6 -> 1
    // Let's do an intermediate check
    REQUIRE(((((((0.0 + 8.0) * 3.0) + 7.0) * 6.0) - 9.0) * 2.0) == singleFiberArg->counter);

    // Now run the rest of the sequence
    singleFiberArg->counter *= 4.0;
    tasks::fiber::switch_to(singleFiberArg->first_fiber, singleFiberArg->fifth_fiber);

    // Return from fifth
    singleFiberArg->counter += 1.0;
    tasks::fiber::switch_to(singleFiberArg->first_fiber, singleFiberArg->third_fiber);

    // We should never get here
    FAIL();
}

void fp_second_level_fiber_start(void* arg)
{
    auto* singleFiberArg = reinterpret_cast<fp_multiple_fiber_arg*>(arg);

    singleFiberArg->counter *= 3.0;
    tasks::fiber::switch_to(singleFiberArg->second_fiber, singleFiberArg->third_fiber);

    // Return from third
    singleFiberArg->counter += 9.0;
    tasks::fiber::switch_to(singleFiberArg->second_fiber, singleFiberArg->fourth_fiber);

    // Return from fourth
    singleFiberArg->counter += 7.0;
    tasks::fiber::switch_to(singleFiberArg->second_fiber, singleFiberArg->fifth_fiber);

    // We should never get here
    FAIL();
}

void fp_third_level_fiber_start(void* arg)
{
    auto* singleFiberArg = reinterpret_cast<fp_multiple_fiber_arg*>(arg);

    singleFiberArg->counter += 7.0;
    tasks::fiber::switch_to(singleFiberArg->third_fiber, singleFiberArg->fourth_fiber);

    // Return from first
    singleFiberArg->counter *= 3.0;
    tasks::fiber::switch_to(singleFiberArg->third_fiber, singleFiberArg->second_fiber);

    // Return from fifth
    singleFiberArg->counter *= 6.0;
    tasks::fiber::switch_to(singleFiberArg->third_fiber, singleFiberArg->sixth_fiber);

    // We should never get here
    FAIL();
}

void fp_fourth_level_fiber_start(void* arg)
{
    auto* singleFiberArg = reinterpret_cast<fp_multiple_fiber_arg*>(arg);

    singleFiberArg->counter *= 6.0;
    tasks::fiber::switch_to(singleFiberArg->fourth_fiber, singleFiberArg->fifth_fiber);

    // Return from second
    singleFiberArg->counter += 8.0;
    tasks::fiber::switch_to(singleFiberArg->fourth_fiber, singleFiberArg->sixth_fiber);

    // Return from sixth
    singleFiberArg->counter *= 5.0;
    tasks::fiber::switch_to(singleFiberArg->fourth_fiber, singleFiberArg->second_fiber);

    // We should never get here
    FAIL();
}

void fp_fifth_level_fiber_start(void* arg)
{
    auto* singleFiberArg = reinterpret_cast<fp_multiple_fiber_arg*>(arg);

    singleFiberArg->counter -= 9.0;
    tasks::fiber::switch_to(singleFiberArg->fifth_fiber, singleFiberArg->sixth_fiber);

    // Return from first
    singleFiberArg->counter *= 5.0;
    tasks::fiber::switch_to(singleFiberArg->fifth_fiber, singleFiberArg->first_fiber);

    // Return from second
    singleFiberArg->counter += 1.0;
    tasks::fiber::switch_to(singleFiberArg->fifth_fiber, singleFiberArg->third_fiber);

    // We should never get here
    FAIL();
}

void fp_sixth_level_fiber_start(void* arg)
{
    auto* singleFiberArg = reinterpret_cast<fp_multiple_fiber_arg*>(arg);

    singleFiberArg->counter *= 2.0;
    tasks::fiber::switch_to(singleFiberArg->sixth_fiber, singleFiberArg->first_fiber);

    // Return from fourth
    singleFiberArg->counter -= 9.0;
    tasks::fiber::switch_to(singleFiberArg->sixth_fiber, singleFiberArg->fourth_fiber);

    // Return from third
    singleFiberArg->counter -= 3.0;
    tasks::fiber::switch_to(singleFiberArg->sixth_fiber, singleFiberArg->main_fiber);

    // We should never get here
    FAIL();
}

void fp_main_level_fiber_state(void* arg)
{
    auto* singleFiberArg = reinterpret_cast<fp_multiple_fiber_arg*>(arg);
    tasks::fiber::switch_to(singleFiberArg->main_fiber, singleFiberArg->first_fiber);
}


TEST_CASE("Floating Point Fiber Switch", "[fiber]")
{
    fp_multiple_fiber_arg arg;
    arg.counter = 0ULL;
    arg.first_fiber.create(commit_size, reserve_size, fp_first_level_fiber_start, &arg);
    arg.second_fiber.create(commit_size, reserve_size, fp_second_level_fiber_start, &arg);
    arg.third_fiber.create(commit_size, reserve_size, fp_third_level_fiber_start, &arg);
    arg.fourth_fiber.create(commit_size, reserve_size, fp_fourth_level_fiber_start, &arg);
    arg.fifth_fiber.create(commit_size, reserve_size, fp_fifth_level_fiber_start, &arg);
    arg.sixth_fiber.create(commit_size, reserve_size, fp_sixth_level_fiber_start, &arg);
    arg.main_fiber.create_from_thread_and_run(fp_main_level_fiber_state, &arg);

    // The order should be:
    // Main -> 1 -> 2 -> 3 -> 4 -> 5 -> 6 -> 1 -> 5 -> 1 -> 3 -> 2 -> 4 -> 6 -> 4 -> 2 -> 5 -> 3 -> 6 -> Main

    REQUIRE(((((((((((((((((((0.0 + 8.0) * 3.0) + 7.0) * 6.0) - 9.0) * 2.0) * 4.0) * 5.0) + 1.0) * 3.0) + 9.0) + 8.0) - 9.0) * 5.0) + 7.0) + 1.0) * 6.0) - 3.0) == arg.counter);
}