// This file is a part of xray-ng engine
//

#include "catch/catch.hpp"
#include "corlib/memory/memory_crt_allocator.h"
#include "corlib/memory/memory_aligned_allocator.h"
#include "corlib/memory/allocator_macro.h"

using namespace xr;

TEST_CASE("aligned_allocator tests")
{
    XR_CONSTEXPR_CPP14_OR_CONST size_t initial = XR_KILOBYTES_TO_BYTES(128);
    XR_CONSTEXPR_CPP14_OR_CONST size_t size = XR_KILOBYTES_TO_BYTES(256);

    memory::crt_allocator allocator();
    memory::aligned_allocator<XR_DEFAULT_MACHINE_ALIGNMENT> aligned_allocator(allocator);

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