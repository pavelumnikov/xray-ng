// This file is a part of xray-ng engine
//

#include "catch/catch.hpp"
#include "corlib/utils/ptr.h"

using namespace xr;

namespace
{
// Function to screw up initial data
void f(int* i)
{
    *i += 1;
}
} // namespace anonymous

TEST_CASE("Basic test")
{
    utils::owner<int*> p { new int(120) };
    CHECK(*p == 120);
    f(p);
    CHECK(*p == 121);
    delete p;
}