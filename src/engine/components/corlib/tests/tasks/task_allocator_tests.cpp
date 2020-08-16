// This file is a part of xray-ng engine
//

#include "catch/catch.hpp"
#include "../../sources/tasks/allocator.h"

struct simple_node
{
    int value { 0 };
}; // struct simple_node

TEST_CASE("Allocate and free task_bases")
{
    xr::tasks::allocator<simple_node, 10> allocator {};

    // allocate some tasks
    simple_node* t1 = allocator.take_available();
    simple_node* t2 = allocator.take_available();

    REQUIRE(t1 != nullptr);
    REQUIRE(t2 != nullptr);
    REQUIRE(!allocator.check_all_free());

    // free allocated tasks
    allocator.put_back(t1);
    allocator.put_back(t2);

    REQUIRE(allocator.check_all_free());
}
