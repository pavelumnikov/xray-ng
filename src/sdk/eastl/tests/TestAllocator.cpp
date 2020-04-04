/////////////////////////////////////////////////////////////////////////////
// Copyright (c) Electronic Arts Inc. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#include "EASTLTest.h"
#include <EASTL/allocator.h>
#include <EASTL/fixed_allocator.h>
#include <EASTL/EAStdC/EAString.h>
#include <EASTL/EAStdC/EAAlignment.h>

namespace
{

struct fixed_pool_reference
{
public:
    fixed_pool_reference(const char* = NULL)
    {
        mpFixedPool = NULL;
    }

    fixed_pool_reference(eastl::fixed_pool& fixedPool)
    {
        mpFixedPool = &fixedPool;
    }

    fixed_pool_reference(const fixed_pool_reference& x)
    {
        mpFixedPool = x.mpFixedPool;
    }

    fixed_pool_reference& operator=(const fixed_pool_reference& x)
    {
        mpFixedPool = x.mpFixedPool;
        return *this;
    }

    void* allocate(size_t /*n*/, int /*flags*/ = 0)
    {
        return mpFixedPool->allocate();
    }

    void* allocate(size_t /*n*/, size_t /*alignment*/, size_t /*offset*/, int /*flags*/ = 0)
    {
        return mpFixedPool->allocate();
    }

    void deallocate(void* p, size_t /*n*/)
    {
        return mpFixedPool->deallocate(p);
    }

    const char* get_name() const
    {
        return "fixed_pool_reference";
    }

    void set_name(const char* /*pName*/)
    {}

protected:
    friend bool operator==(const fixed_pool_reference& a, const fixed_pool_reference& b);
    friend bool operator!=(const fixed_pool_reference& a, const fixed_pool_reference& b);

    eastl::fixed_pool* mpFixedPool;
};


inline bool operator==(const fixed_pool_reference& a, const fixed_pool_reference& b)
{
    return (a.mpFixedPool == b.mpFixedPool);
}

inline bool operator!=(const fixed_pool_reference& a, const fixed_pool_reference& b)
{
    return (a.mpFixedPool != b.mpFixedPool);
}

struct TestClass
{
    mutable int mX;

    TestClass() : mX(37)
    {}

    void Increment()
    {
        mX++;
    }

    void IncrementConst() const
    {
        mX++;
    }

    int MultiplyBy(int x)
    {
        return mX * x;
    }

    int MultiplyByConst(int x) const
    {
        return mX * x;
    }
};

}

TEST_CASE("TestAllocator")
{
    using namespace eastl;
    const size_t node_count = 256;

    SECTION("Test: fixed allocator with stack buffer (only allocate)")
    {
        int intNodes[node_count] = { 0 };
        fixed_pool my_pool { intNodes, sizeof(intNodes), sizeof(int), 16 };

        REQUIRE(my_pool.mnCurrentSize == 0);
        int* first_ptr = static_cast<int*>(my_pool.allocate());
        REQUIRE(my_pool.mnCurrentSize == 1);
    }

    SECTION("Test: swap allocators")
    {
        InstanceAllocator a { nullptr, (uint8_t)111 }, b { nullptr, (uint8_t)222 };
        eastl::swap(a, b);

        REQUIRE(a.mInstanceId == 222);
        REQUIRE(b.mInstanceId == 111);

        REQUIRE(EA::StdC::Strcmp(a.get_name(), "InstanceAllocator 222") == 0);
        REQUIRE(EA::StdC::Strcmp(b.get_name(), "InstanceAllocator 111") == 0);
    }

    SECTION("Test: allocator and alignment")
    {
        auto testAllocatorAlignment = [](size_t requestedSize, size_t requestedAlignment, size_t requestedOffset)
        {
            CountingAllocator::resetCount();
            CountingAllocator a;

            void* p = allocate_memory(a, requestedSize, requestedAlignment, requestedOffset);

            REQUIRE(p != nullptr);
            REQUIRE(EA::StdC::IsAligned(p, requestedAlignment));

            a.deallocate(p, requestedSize);
            REQUIRE(CountingAllocator::getActiveAllocationSize() == 0);
        };

        testAllocatorAlignment(100, 1, 0);
        testAllocatorAlignment(100, 2, 0);
        testAllocatorAlignment(100, 4, 0);
        testAllocatorAlignment(100, 8, 0);
        testAllocatorAlignment(100, 16, 0);

        testAllocatorAlignment(100, 1, 16);
        testAllocatorAlignment(100, 2, 16);
        testAllocatorAlignment(100, 4, 16);
        testAllocatorAlignment(100, 8, 16);
        testAllocatorAlignment(100, 16, 16);
    }
}
