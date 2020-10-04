/*

  Copyright (c) 2018, Pavel Umnikov
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

#include <catch/catch.hpp>
#include "../sources/tasks/pool.h"
#include "../sources/tasks/scheduler.h"
#include "corlib/memory/memory_crt_allocator.h"

using namespace xr;
static memory::crt_allocator crt_allocator;

TEST_CASE("Tasks: local pool basic test", "az::tasks::pool")
{
    SECTION("Basic tests")
    {
        tasks::pool const single_pool { crt_allocator };
        REQUIRE(single_pool.is_empty_slot());
    }
}

TEST_CASE("Tasks: local pool task allocation tests (with optimistic task get)", "az::tasks::pool")
{
    SECTION("Task allocation tests")
    {
        tasks::pool single_pool { crt_allocator };
        tasks::task t {};

        REQUIRE(single_pool.is_starving<threading::memory_order::relaxed>());

        tasks::task* tptr = &t;
        single_pool.push_front(&tptr, 1);

        REQUIRE(!single_pool.is_starving<threading::memory_order::relaxed>());
        REQUIRE(single_pool.is_empty_slot());
        REQUIRE(single_pool.is_empty_or_starved<threading::memory_order::relaxed>());

        auto out_task = single_pool.pop_back_optimistic();
        REQUIRE(single_pool.is_empty_slot());
        REQUIRE(single_pool.is_starving<threading::memory_order::relaxed>());
        REQUIRE(single_pool.is_empty_or_starved<threading::memory_order::relaxed>());
        REQUIRE(out_task == tptr);
    }
}

TEST_CASE("Tasks: local pool task allocation tests (with pessimistic task get)", "az::tasks::pool")
{
    SECTION("Task allocation tests")
    {
        uint32_t constexpr arena_concurrency = 1;

        tasks::pool single_pool { crt_allocator };
        tasks::task t {};

        // head equals tail at this point, because no task here
        REQUIRE(single_pool.is_starving<threading::memory_order::relaxed>());

        tasks::task* tptr = &t;
        single_pool.push_front(&tptr, 1);

        // no concurrency, no slot contention - it must be empty
        REQUIRE(single_pool.is_empty_slot());
        // after the first insertion we must ensure that pool is properly
        // adjusted for new tail and head
        REQUIRE(!single_pool.is_starving<threading::memory_order::relaxed>());
        // this must be empty, but not starved; this is checked earlier 
        REQUIRE(single_pool.is_empty_or_starved<threading::memory_order::relaxed>());

        auto out_task = single_pool.pop_back_pessimistic(arena_concurrency);
        // no concurrency, no slot contention - it must be empty
        REQUIRE(single_pool.is_empty_slot());
        // on stealing we are taking tasks from head, so we cannot say about starvation
        REQUIRE(!single_pool.is_starving<threading::memory_order::relaxed>());
        // this must be empty, but not starved; this is checked earlier
        REQUIRE(single_pool.is_empty_or_starved<threading::memory_order::relaxed>());
        REQUIRE(out_task == tptr);
    }
}