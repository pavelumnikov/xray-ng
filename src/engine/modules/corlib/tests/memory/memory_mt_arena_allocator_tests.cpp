#include "catch/catch.hpp"
#include "corlib/memory/memory_mt_arena_allocator.h"
#include "corlib/memory/allocator_macro.h"

using namespace xr;

TEST_CASE("mt_arena_allocator tests")
{
    constexpr size_t initial = 64_kb;
    constexpr size_t size = 128_kb;

    memory::mt_arena_allocator allocator;
    allocator.initialize(size, initial);

    SECTION("malloc test")
    {
        size_t s = 64; // 64 bytes to allocated
        pvoid block = XR_ALLOCATE_MEMORY(allocator, s, "test to allocate");
        REQUIRE(block != nullptr);
    }

    SECTION("malloc-free test")
    {
        size_t s = 64; // 64 bytes to allocated
        pvoid block = XR_ALLOCATE_MEMORY(allocator, s, "test to allocate");
        REQUIRE(block != nullptr);
        XR_DEALLOCATE_MEMORY(allocator, block);
    }

    SECTION("can_allocate (success) test")
    {
        size_t s = 128; // 128 bytes to allocated
        bool result = allocator.can_allocate_block(s);
        REQUIRE(result == true);
    }
}