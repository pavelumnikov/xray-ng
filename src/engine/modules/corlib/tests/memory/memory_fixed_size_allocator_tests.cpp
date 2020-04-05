#include "catch/catch.hpp"
#include "corlib/memory/memory_crt_allocator.h"

using namespace xr;

TEST_CASE("fixed_size_allocator tests")
{
    // TODO: make real fixed_allocator
    memory::crt_allocator allocator;

    SECTION("malloc test")
    {
        size_t s = 128; // 128 bytes to allocated
        pvoid block = XR_ALLOCATE_MEMORY(allocator, s, "test to allocate");
        REQUIRE(block != nullptr);
    }

    SECTION("malloc-free test")
    {
        size_t s = 64; // 128 bytes to allocated
        pvoid block = XR_ALLOCATE_MEMORY(allocator, s, "test to allocate");
        REQUIRE(block != nullptr);
        XR_DEALLOCATE_MEMORY(allocator, block);
    }

    SECTION("can_allocate test")
    {
        size_t s = 128; // 128 bytes to allocated
        bool result = allocator.can_allocate_block(s);
        REQUIRE(result == true);
    }
}