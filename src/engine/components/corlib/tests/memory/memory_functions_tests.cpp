// This file is a part of xray-ng engine
//

#include "catch/catch.hpp"
#include "corlib/memory/memory_functions.h"

using namespace xr;

struct test_struct
{
    int i { 10 };
    int a { 20 };
};

TEST_CASE("memory functions tests")
{
    SECTION("zero memory")
    {
        test_struct block {};
        memory::zero(&block, sizeof(block));
        REQUIRE(block.i == 0);
        REQUIRE(block.a == 0);
    }

    SECTION("copy memory")
    {
        constexpr int const_val_A = 50;
        constexpr int const_val_B = 100;

        test_struct blockA { const_val_A, const_val_B };
        test_struct blockB {};

        memory::copy(&blockB, sizeof(blockB), &blockA, sizeof(blockA));

        REQUIRE(blockB.i == const_val_A);
        REQUIRE(blockB.a == const_val_B);
    }
}